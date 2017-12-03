#include "Game.h"
#include "GameState.h"
//#include "ToMenuTransition.h"
#include "IntroState.h"

Game::Game() :
	m_bestVideoMode(sf::VideoMode::getFullscreenModes()[0]),
#ifdef __ANDROID__
	m_currentVideoMode(sf::VideoMode::getFullscreenModes()[0]),
	m_settings(0, 0, 1),
	m_window(m_currentVideoMode, "Game", sf::Style::Fullscreen, m_settings),
#else
	m_currentVideoMode(//sf::VideoMode(1302, 600) // <- iPhone X 2.17:1
					   //sf::VideoMode(1200, 600) // <- 18:9
					   //sf::VideoMode(1067, 600) // <- 16:9
					   //sf::VideoMode(750, 600) // <- 5:4
					   //sf::VideoMode(1024, 768) // <- 4:3
					   sf::VideoMode::getFullscreenModes()[4]),
	m_settings(0, 0, 4),
	m_window(m_currentVideoMode, "Game", sf::Style::None, m_settings),
#endif
	m_background(new AcceptanceBackground(sf::Vector2u(m_currentVideoMode.width, m_currentVideoMode.height))),
	m_viewZoom(INITIAL_VIEW_ZOOM),
	m_world(this),
	m_player(VIEW_CENTER.x, VIEW_CENTER.y, 20, 20),
	m_playerInputHandler(sf::IntRect(0, 0,
									 m_window.getSize().x / 2,
									 m_window.getSize().y),
						 sf::IntRect(m_window.getSize().x / 2, 0,
									 m_window.getSize().x / 2,
									 m_window.getSize().y),
						 sf::Keyboard::Left,
						 sf::Keyboard::Right),
	m_enemySpawner(&m_gameObjects)
{
#ifndef NDEBUG
	m_window.setVerticalSyncEnabled(true);
#endif // DEBUG
	m_window.setKeyRepeatEnabled(false);

	m_fileManager.loadFont("Buttons", "GameButtons.ttf");
	m_fileManager.loadFont("Helvetica", "HelveticaNeueCyr.otf");
	m_fileManager.loadFont("Futurica", "FuturicaBs.ttf");

	m_fileManager.loadSpawnPresets("SpawnPresets.txt");
	m_enemySpawner.setSpawnPresets(m_fileManager.getSpawnPresets());

	if (sf::Shader::isAvailable())
	{
		m_fileManager.loadShader("WhiteRadialGradient",
								 "RadialGradient.frag",
								 "RadialGradient.vert");

		m_world.setShader(m_fileManager.getShader("WhiteRadialGradient"));
	}

	updateRender(m_currentVideoMode);

	m_music.openFromFile("Courtesy.ogg");
	m_music.setLoop(true);
	m_music.setVolume(50.f);
	m_music.play();

	m_clock.restart();

	m_player.addObserver(m_background);

	SharedContext context(&m_fileManager,
						  &m_view,
						  &m_gameObjects,
						  &m_enemySpawner,
						  &m_player,
						  &m_world,
						  m_background,
						  &m_bestTime);

	m_state = new IntroState(this, context);


#ifndef NDEBUG
	m_debugOverlay.setGame(this);
	m_debugOverlay.setFont(m_fileManager.getFont("Futurica"));
#endif
}

Game::~Game()
{
}

sf::VideoMode Game::getCurrentVideoMode() const
{
	return m_currentVideoMode;
}

sf::VideoMode Game::getBestVideoMode() const
{
	return m_bestVideoMode;
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

sf::Vector2i Game::mapCoordsToPixel(const sf::Vector2f & point) const
{
	return m_window.mapCoordsToPixel(point);
}

sf::Vector2i Game::mapCoordsToPixel(const sf::Vector2f & point, const sf::View & view) const
{
	return m_window.mapCoordsToPixel(point, view);
}

sf::Vector2f Game::mapPixelToCoords(const sf::Vector2i & point) const
{
	return m_window.mapPixelToCoords(point);
}

sf::Vector2f Game::mapPixelToCoords(const sf::Vector2i & point, const sf::View & view) const
{
	return m_window.mapPixelToCoords(point, view);
}

void Game::setMusicVolume(float volume)
{
	m_music.setVolume(volume);
}

float Game::getMusicVolume() const
{
	return m_music.getVolume();
}

bool Game::hasFocus() const
{
	return m_window.hasFocus();
}

void Game::changeState(GameState* state)
{
	delete m_state;
	m_state = state;
}

void Game::gameLoop()
{
	while (m_window.isOpen())
	{
		handleInput();

		if (m_state == nullptr)
		{
			continue;
		}

		update();
		render();
		restartClock();
	}
}

void Game::close()
{
	// Delete current background
	delete m_background;
	m_background = nullptr;

	// Deallocate current state memory
	m_state->clear();

	// Delete current state
	delete m_state;
	m_state = nullptr;

	// Close app
	m_window.close();
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
	
	m_world.updateBounds(sf::FloatRect(VIEW_CENTER.x - m_defaultViewSize.x / 2,
									   VIEW_CENTER.y - m_defaultViewSize.y / 2,
									   m_defaultViewSize.x,
									   m_defaultViewSize.y));
}

void Game::handleInput()
{
	while (m_window.pollEvent(m_event))
	{
		Command* command = m_playerInputHandler.handleInput(m_event);

		if (command != nullptr)
		{
			command->execute(m_player);
		}

		m_state->handleInput(m_event);

		switch (m_event.type)
		{
		case sf::Event::Closed:
			close();
			return;

		case sf::Event::Resized:
			updateRender(m_event.size.width, m_event.size.height);
			break;

		case sf::Event::LostFocus:
			m_window.setFramerateLimit(10);
			m_window.setActive(false);
			break;

		case sf::Event::GainedFocus:
			m_window.setFramerateLimit(0);
			m_window.setActive(true);
			break;

		default:
			break;
		}
	}
}

void Game::update()
{
	while (m_elapsed.asSeconds() >= FRAME_TIME)
	{
		m_background->update(sf::seconds(FRAME_TIME));
		m_world.update(sf::seconds(FRAME_TIME), &m_player, &m_view);

		// update current state
		m_state->update(sf::seconds(FRAME_TIME));

		// delete all dead objects
		for (auto i = m_gameObjects.begin(); i != m_gameObjects.end();)
		{
			if (!(*i)->isAlive())
			{
				delete (*i);
				(*i) = nullptr;

				i = m_gameObjects.erase(i);
			}
			else
			{
				i++;
			}
		}

		m_elapsed -= sf::seconds(FRAME_TIME);

#ifndef NDEBUG
		m_updatesCounter++;
#endif // _DEBUG
	}

#ifndef NDEBUG

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

	m_state->draw(m_window);

	m_window.setView(m_window.getDefaultView());

#ifndef NDEBUG
	m_window.draw(m_debugOverlay);
	m_framesCounter++;
#endif
	m_window.display();
}

void Game::restartClock()
{
	m_elapsed += m_clock.restart();
}
