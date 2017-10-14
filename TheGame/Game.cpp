#include "Game.h"
#include "GameState.h"
#include "Definitions.hpp"

Game::Game() :
	m_bestVideoMode(sf::VideoMode::getFullscreenModes()[0]),
#ifdef __ANDROID__
	m_currentVideoMode(sf::VideoMode::getFullscreenModes()[0]),
	m_settings(0, 0, 0),
	m_window(m_currentVideoMode, "Game", sf::Style::Default, m_settings),
	//
#else
	m_currentVideoMode(sf::VideoMode::getFullscreenModes()[0]),
	m_settings(0, 0, 4),
	m_window(m_currentVideoMode, "Game", sf::Style::Default, m_settings),
#endif
	m_background(sf::Vector2u(m_currentVideoMode.width, m_currentVideoMode.height)),
	m_viewZoom(INITIAL_VIEW_ZOOM),
	m_world(this),
	m_player(VIEW_CENTER.x, VIEW_CENTER.y, 20, 20),
	m_titleColor(INITIAL_TITLE_COLOR)
{
#ifdef _DEBUG
	//m_window.setFramerateLimit(60);
#endif // DEBUG
	m_window.setKeyRepeatEnabled(false);

	m_fileManager.loadFont("Buttons", "GameButtons.ttf");
	m_fileManager.loadFont("Helvetica", "HelveticaNeueCyr.otf");
	m_fileManager.loadFont("Futurica", "FuturicaBs.ttf");

	m_fileManager.loadSound("Space", "");
	m_fileManager.loadSound("Courtesy", "");
	m_fileManager.loadSound("Otis", "Courtesy.ogg");
	m_fileManager.loadSound("RecordRewind", "RecordRewind.ogg");

	m_fileManager.loadSpawnPresets("SpawnPresets.txt");

	if (sf::Shader::isAvailable())
	{
		m_fileManager.loadShader("WhiteRadialGradient", "RadialGradient.frag", "RadialGradient.vert");

		m_world.setShader(m_fileManager.getShader("WhiteRadialGradient"));
	}

	updateRender(m_currentVideoMode);

	m_title.setFont(*m_fileManager.getFont("Helvetica"));
	m_title.setString(GAME_NAME);
	m_title.setCharacterSize(static_cast<unsigned int>(CHARACTER_SIZE_FACTOR * m_bestVideoMode.height));
	m_title.setOrigin(m_title.getLocalBounds().width / 2, m_title.getGlobalBounds().height / 2);
	m_title.setScale(float(m_currentVideoMode.width) / float(m_bestVideoMode.width),
					 float(m_currentVideoMode.width) / float(m_bestVideoMode.width));
	m_title.setPosition(static_cast<float>(m_currentVideoMode.width) / 2,
						TITLE_Y_POSITION_FACTOR *
						static_cast<float>(m_currentVideoMode.height));
	m_title.setFillColor(m_titleColor);

	m_clock.restart();
#if defined(_DEBUG) || defined(__ANDROID__)
	m_debugOverlay.setGame(this);
	m_debugOverlay.setFont(m_fileManager.getFont("Futurica"));
#endif
}

Game::~Game()
{
	clearStates();
}

sf::RenderWindow * Game::getWindow()
{
	return &m_window;
}

sf::View * Game::getView()
{
	return &m_view;
}

const sf::Vector2f & Game::getDefaultViewSize() const
{
	return m_defaultViewSize;
}

float Game::getViewZoom() const
{
	return m_viewZoom;
}

void Game::setViewZoom(float newZoom)
{
	m_viewZoom = newZoom;

	m_view.setSize(m_defaultViewSize * newZoom);
}

FileManager * Game::getFileManager()
{
	return &m_fileManager;
}

World * Game::getWorld()
{
	return &m_world;
}

Player * Game::getPlayer()
{
	return &m_player;
}

sf::Color Game::getTitleColor() const
{
	return m_titleColor;
}

void Game::setTitleColor(sf::Color color)
{
	m_titleColor = color;
	m_title.setFillColor(m_titleColor);
}

sf::Vector2i Game::getPixelMousePosition() const
{
	return m_mousePosition;
}

sf::Vector2i Game::getPixelMousePosition(const sf::View & view) const
{
	return m_window.mapCoordsToPixel(sf::Vector2f(m_mousePosition), view);
}

sf::Vector2f Game::getWorldMousePosition() const
{
	return m_window.mapPixelToCoords(m_mousePosition);
}

sf::Vector2f Game::getWorldMousePosition(const sf::View & view) const
{
	return m_window.mapPixelToCoords(m_mousePosition, view);
}

void Game::pushState(GameState* state)
{
	m_states.push(state);
}

void Game::popState()
{
	delete m_states.top();
	m_states.top() = nullptr;

	m_states.pop();
}

void Game::changeState(GameState* state)
{
	if (!m_states.empty())
	{
		popState();
	}
	pushState(state);
}

GameState* Game::peekState()
{
	if (m_states.empty())
	{
		return nullptr;
	}
	return m_states.top();
}

void Game::gameLoop()
{
	while (m_window.isOpen())
	{
		if (m_window.hasFocus())
		{
			handleInput();

			if (peekState() == nullptr)
			{
				continue;
			}

			update();
			render();
			restartClock();
		}
		else
		{
			m_window.waitEvent(m_event);
		}
	}
}

void Game::close()
{
	clearStates();
	m_window.close();
}

void Game::clearStates()
{
	while (!m_states.empty())
	{
		popState();
	}
}

void Game::updateRender(sf::VideoMode videoMode)
{
	updateRender(videoMode.width, videoMode.height, videoMode.bitsPerPixel);
}

void Game::updateRender(unsigned int width, unsigned int height, unsigned int bitsPerPixel)
{
	m_currentVideoMode = sf::VideoMode(width, height, bitsPerPixel);
	m_aspectRatio = float(width) / float(height);
	m_defaultViewSize = sf::Vector2f(WORLD_SIZE.x, WORLD_SIZE.x / m_aspectRatio);
	m_view.setSize(m_defaultViewSize * m_viewZoom);
	m_view.setCenter(VIEW_CENTER);

	m_title.setScale(float(m_currentVideoMode.width) / float(m_bestVideoMode.width),
					 float(m_currentVideoMode.width) / float(m_bestVideoMode.width));

	m_world.updateBounds(sf::FloatRect(VIEW_CENTER.x - m_defaultViewSize.x / 2,
									   VIEW_CENTER.y - m_defaultViewSize.y / 2,
									   m_defaultViewSize.x,
									   m_defaultViewSize.y));
}

void Game::handleInput()
{
	while (m_window.pollEvent(m_event))
	{
		peekState()->handleInput(m_event);

		switch (m_event.type)
		{
		case sf::Event::Closed:
			close();
			return;

		case sf::Event::Resized:
			updateRender(m_event.size.width, m_event.size.height);
			break;

		case sf::Event::MouseMoved:
			m_mousePosition = sf::Mouse::getPosition(m_window);
			break;

		case sf::Event::TouchMoved:
			m_mousePosition = sf::Touch::getPosition(0, m_window);
			break;

		case sf::Event::TouchBegan:
			m_mousePosition = sf::Touch::getPosition(0, m_window);
			break;

		default:
			break;
		}
	}
}

void Game::update()
{
	float frameTime = 1.f / 60.f;

	while (m_elapsed.asSeconds() >= frameTime)
	{
		m_background.update(sf::seconds(frameTime));
		m_world.update(sf::seconds(frameTime));
		peekState()->update(sf::seconds(frameTime));
		m_elapsed -= sf::seconds(frameTime);

#if defined(_DEBUG) || defined(__ANDROID__)
		m_updatesCounter++;
#endif // _DEBUG
	}

#if defined(_DEBUG) || defined(__ANDROID__)

	m_debugOverlay.update();

	if (m_titleClock.getElapsedTime().asSeconds() >= 1)
	{
		m_ups = m_updatesCounter;
		m_fps = m_framesCounter;

		m_updatesCounter = 0;
		m_framesCounter = 0;

		m_titleClock.restart();
	}
#endif
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	m_window.draw(m_background);

	m_window.setView(m_view);

	m_window.draw(m_world);
	m_window.draw(m_player);

	peekState()->draw(m_window);

	m_window.setView(m_window.getDefaultView());

	m_window.draw(m_title);

#if defined(_DEBUG) || defined(__ANDROID__)
	m_window.draw(m_debugOverlay);
	m_framesCounter++;
#endif
	m_window.display();
}

void Game::restartClock()
{
	m_elapsed += m_clock.restart();
}
