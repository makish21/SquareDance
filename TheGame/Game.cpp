#include "Game.h"
#include "GameState.h"
#include "Definitions.hpp"

Game::Game() :
#ifdef __ANDROID__
	m_videoMode(sf::VideoMode::getFullscreenModes()[0]),
	m_settings(0, 0, 0),
	m_window(m_videoMode, "Game", sf::Style::Default, m_settings),
#else
	m_videoMode(sf::VideoMode::getFullscreenModes()[0]),
	m_settings(0, 0, 4),
	m_window(m_videoMode, "Game", sf::Style::Resize, m_settings),
#endif
	m_viewZoom(8.f),
	m_world(this),
	m_player(VIEW_CENTER.x, VIEW_CENTER.y, 20, 20),
	m_titleColor(242, 227, 160, 0)
{
#ifdef _DEBUG
	//m_window.setFramerateLimit(120);
#endif // DEBUG
	m_window.setKeyRepeatEnabled(false);

	m_fileManager.loadFont("Buttons", "GameButtons.ttf");
	m_fileManager.loadFont("Helvetica", "HelveticaNeueCyr.otf");
	m_fileManager.loadFont("Futurica", "FuturicaBs.ttf");

	m_fileManager.loadSound("Space", "");
	m_fileManager.loadSound("Courtesy", "");
	m_fileManager.loadSound("Otis", "Courtesy.ogg");

	m_fileManager.loadSpawnPresets("SpawnPresets.txt");

	if (sf::Shader::isAvailable())
	{
		m_fileManager.loadShader("WhiteRadialGradient", "RadialGradient.frag", "RadialGradient.vert");

		m_world.setShader(m_fileManager.getShader("WhiteRadialGradient"));
	}

	updateRender(m_videoMode);

	m_title.setFont(*m_fileManager.getFont("Helvetica"));
	m_title.setString(GAME_NAME);
	m_title.setCharacterSize(static_cast<unsigned int>(CHARACTER_SIZE_FACTOR * m_videoMode.height));
	m_title.setOrigin(m_title.getLocalBounds().width / 2, m_title.getGlobalBounds().height / 2);
	m_title.setPosition(static_cast<float>(m_videoMode.width) / 2,
						TITLE_Y_POSITION_FACTOR *
						static_cast<float>(m_videoMode.height));
	m_title.setFillColor(m_titleColor);


#if defined(_DEBUG) || defined(__ANDROID__)
	m_debugText.setFont(*m_fileManager.getFont("Futurica"));
	m_debugText.setCharacterSize(15);
	m_debugText.setPosition(10.f, 10.f);
	m_debugText.setFillColor(sf::Color::White);

	m_dBitsPerPixel = "Bits per pixel: " + std::to_string(m_videoMode.bitsPerPixel) + '\n';
	m_dUpdates = "Updates: 0\n";
	m_dFrames = "Frames: 0\n";
#endif // _DEBUG
}

Game::~Game()
{
	clearStates();
}

sf::RenderTarget * Game::getWindow()
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

sf::Uint8 Game::getTitleAlpha() const
{
	return m_titleColor.a;
}

void Game::setTitleAlpha(sf::Uint8 newAlpha)
{
	m_titleColor.a = newAlpha;

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

bool Game::pollEvent(sf::Event & event)
{
	return m_window.pollEvent(event);
}

void Game::handleEvent(sf::Event & event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		close();
		return;

	case sf::Event::Resized:
		updateRender(event.size.width, event.size.height);
		break;

	default:
		break;
	}
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
	m_videoMode = sf::VideoMode(width, height, bitsPerPixel);
	m_aspectRatio = float(width) / float(height);
	m_defaultViewSize = sf::Vector2f(WORLD_SIZE.x, WORLD_SIZE.x / m_aspectRatio);
	m_view.setSize(m_defaultViewSize * m_viewZoom);
	m_view.setCenter(VIEW_CENTER);

	m_world.updateBounds(sf::FloatRect(VIEW_CENTER.x - m_defaultViewSize.x / 2,
									   VIEW_CENTER.y - m_defaultViewSize.y / 2,
									   m_defaultViewSize.x,
									   m_defaultViewSize.y));
}

void Game::handleInput()
{
#ifdef __ANDROID__
	if (sf::Touch::isDown(0))
	{
		m_mousePosition = sf::Touch::getPosition(0, m_window);
}
#else
	m_mousePosition = sf::Mouse::getPosition(m_window);
#endif // __ANDROID__

	peekState()->handleInput(m_event);
}

void Game::update()
{
	float frameTime = 1.f / 60.f;

	while (m_elapsed.asSeconds() >= frameTime)
	{
		m_world.update(1.4f);
		peekState()->update(1.4f);
		m_elapsed -= sf::seconds(frameTime);

#if defined(_DEBUG) || defined(__ANDROID__)
		m_updates++;
#endif // _DEBUG
	}

#if defined(_DEBUG) || defined(__ANDROID__)
	m_dPlayerPos = "Player position: x: " + std::to_string(static_cast<int>(m_player.getPosition().x)) +
		" y: " + std::to_string(static_cast<int>(m_player.getPosition().y)) + '\n';

	m_dViewSize = "View size: x: " + std::to_string(static_cast<int>(m_view.getSize().x)) +
		" y: " + std::to_string(static_cast<int>(m_view.getSize().y)) + '\n';

	m_dTitleAlpha = "Title alpha: " + std::to_string(m_titleColor.a) + '\n';

	m_dMousePosition = "Mouse position: x: " + std::to_string(getWorldMousePosition(m_view).x) +
		" y: " + std::to_string(getWorldMousePosition(m_view).y) + '\n';

	if (m_titleClock.getElapsedTime().asSeconds() >= 1)
	{
		std::string title = "Game | updates: ";
		title.append(std::to_string(m_updates));
		title.append(" frames: ");
		title.append(std::to_string(m_frames));
		m_window.setTitle(title);

		m_dUpdates = "Updates: " + std::to_string(m_updates) + '\n';
		m_dFrames = "Frames: " + std::to_string(m_frames) + '\n';

		m_updates = 0;
		m_frames = 0;
		m_titleClock.restart();
	}

	m_debugText.setString(m_dBitsPerPixel +
						  m_dUpdates +
						  m_dFrames +
						  m_dPlayerPos +
						  m_dViewSize +
						  m_dTitleAlpha +
						  m_dMousePosition);
#endif
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	m_window.setView(m_view);

	m_window.draw(m_world);
	m_window.draw(m_player);

	peekState()->draw(m_window);

	m_window.setView(m_window.getDefaultView());

	m_window.draw(m_title);

#if defined(_DEBUG) || defined(__ANDROID__)
	m_window.setView(m_window.getDefaultView());
	m_window.draw(m_debugText);

	m_frames++;
#endif
	m_window.display();
}

void Game::restartClock()
{
	m_elapsed += m_clock.restart();
}
