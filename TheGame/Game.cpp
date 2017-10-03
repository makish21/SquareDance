#include "Game.h"
#include "GameState.h"

#define CHARACTER_SIZE_FACTOR 0.1270833333f
#define TITLE_Y_POSITION_FACTOR 0.33125f
#define DEFAULT_VIEW_WITDH 800.f
#define VIEW_CENTER sf::Vector2f(400, 240)

Game::Game() :
#ifdef __ANDROID__
	m_videoMode(sf::VideoMode::getFullscreenModes()[0]),
	m_settings(0, 0, 0),
	m_window(m_videoMode, "Game", sf::Style::Default, m_settings),
#else
	m_videoMode(sf::VideoMode::getFullscreenModes()[6]),
	m_settings(0, 0, 4),
	m_window(m_videoMode, "Game", sf::Style::Resize, m_settings),
#endif
	m_viewZoom(8.f),
	m_aspectRatio(float(m_videoMode.width) / float(m_videoMode.height)),
	m_defaultViewSize(DEFAULT_VIEW_WITDH, DEFAULT_VIEW_WITDH / m_aspectRatio),
	m_view(VIEW_CENTER, m_defaultViewSize * m_viewZoom),
	m_world(sf::FloatRect(0.f, (480.f - static_cast<float>(m_videoMode.height)) / 2,
						  DEFAULT_VIEW_WITDH,
						  static_cast<float>(m_videoMode.height))),
	m_player(VIEW_CENTER.x, VIEW_CENTER.y, 20, 20),
	m_titleColor(242, 227, 160, 0)
{
#ifdef _DEBUG
	m_window.setFramerateLimit(120);
#endif // DEBUG

	m_world.setWindowHeight(m_videoMode.height);

	m_fonts.emplace("Buttons", new sf::Font);
	m_fonts.emplace("Helvetica", new sf::Font);
	m_fonts.emplace("Futurica", new sf::Font);

	m_fonts.at("Buttons")->loadFromFile("GameButtons.ttf");
	m_fonts.at("Helvetica")->loadFromFile("HelveticaNeueCyr.otf");
	m_fonts.at("Futurica")->loadFromFile("FuturicaBs.ttf");

	m_title.setFont(*getFont("Helvetica"));
	m_title.setString(GAME_NAME);
	m_title.setCharacterSize(static_cast<unsigned int>(CHARACTER_SIZE_FACTOR * m_videoMode.height));
	m_title.setOrigin(m_title.getLocalBounds().width / 2, m_title.getGlobalBounds().height / 2);
	m_title.setPosition(static_cast<float>(m_videoMode.width) / 2,
						TITLE_Y_POSITION_FACTOR * 
						static_cast<float>(m_videoMode.height));
	m_title.setFillColor(m_titleColor);

	sf::SoundBuffer buffer;

	m_soundBuffers.emplace("Space", new sf::SoundBuffer(buffer));
	m_soundBuffers.emplace("Courtesy", new sf::SoundBuffer(buffer));
	m_soundBuffers.emplace("Otis", new sf::SoundBuffer(buffer));

#if defined(_DEBUG) || defined(__ANDROID__)
	m_debugText.setFont(*getFont("Futurica"));
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

	for (auto i = m_fonts.begin(); i != m_fonts.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_fonts.clear();

	for (auto i = m_soundBuffers.begin(); i != m_soundBuffers.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_soundBuffers.clear();
}

sf::Window * Game::getWindow()
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

World * Game::getWorld()
{
	return &m_world;
}

sf::Font * Game::getFont(const std::string& name) const
{
	return m_fonts.at(name);
}

sf::SoundBuffer * Game::getSoundBuffer(const std::string & name) const
{
	return m_soundBuffers.at(name);
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

void Game::handleEvent(sf::Event & event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		close();
		return;

	case sf::Event::Resized:
		m_videoMode = sf::VideoMode(event.size.width, event.size.height);
		m_aspectRatio = float(event.size.width) / float(event.size.height);
		m_defaultViewSize = sf::Vector2f(DEFAULT_VIEW_WITDH, DEFAULT_VIEW_WITDH / m_aspectRatio);
		m_view.setSize(m_defaultViewSize * m_viewZoom);
		m_world.setWindowHeight(event.size.height);
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
		m_world.setPlayerPosition(sf::Vector2f(m_player.getPosition().x,
											   m_player.getPosition().y - 
											   (480.f - static_cast<float>(m_videoMode.height)) / 2));
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
	//m_dWorldTransparency = "World transparency: " + std::to_string(m_world.getAlpha()) + '\n';

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
