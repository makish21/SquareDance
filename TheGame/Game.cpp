#include "Game.h"
#include "GameState.h"
//#include "ToMenuTransition.h"
#include "IntroState.h"

Game::Game() :
	m_bestVideoMode(sf::VideoMode::getFullscreenModes()[0]),
#ifdef __ANDROID__
	m_currentVideoMode(sf::VideoMode::getFullscreenModes()[0]),
	m_settings(0, 0, 1),
#else
	m_currentVideoMode(/*sf::VideoMode(320, 240)*/
					   sf::VideoMode::getFullscreenModes()[4]),
	m_settings(0, 0, 4),
#endif
	m_window(m_currentVideoMode, "Game", sf::Style::Default, m_settings),
	m_background(new AcceptanceBackground(sf::Vector2u(m_currentVideoMode.width, m_currentVideoMode.height))),
	m_viewZoom(INITIAL_VIEW_ZOOM),
	m_world(this),
	m_player(VIEW_CENTER.x, VIEW_CENTER.y, 20, 20),
	m_titleColor(INITIAL_TITLE_COLOR),
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
	m_window.setFramerateLimit(60);
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

	m_titleText.setFont(*m_fileManager.getFont("Helvetica"));
	m_titleText.setString(GAME_NAME);
	m_titleText.setCharacterSize(static_cast<unsigned int>(CHARACTER_SIZE_FACTOR * TITLE_CHARACTER_SIZE * m_bestVideoMode.height));
	sf::FloatRect textRect = m_titleText.getLocalBounds();
	m_titleText.setOrigin(textRect.left + textRect.width / 2,
						  textRect.top + textRect.height / 2);
	m_titleText.setScale(float(m_currentVideoMode.width) / float(m_bestVideoMode.width),
						 float(m_currentVideoMode.width) / float(m_bestVideoMode.width));
	m_titleText.setPosition(static_cast<float>(m_currentVideoMode.width) / 2,
							TITLE_Y_POSITION_FACTOR *
							static_cast<float>(m_currentVideoMode.height));
	m_titleText.setFillColor(m_titleColor);

	m_clock.restart();

	m_player.addObserver(m_background);

	SharedContext context(&m_fileManager,
						  &m_view,
						  &m_gameObjects,
						  &m_enemySpawner,
						  &m_player,
						  &m_world,
						  m_background);

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

sf::Color Game::getTitleColor() const
{
	return m_titleColor;
}

void Game::setTitleColor(sf::Color color)
{
	m_titleColor = color;
	m_titleText.setFillColor(m_titleColor);
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

void Game::changeState(GameState* state)
{
	delete m_state;
	m_state = state;
}

void Game::gameLoop()
{
	while (m_window.isOpen())
	{
		if (m_window.hasFocus())
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
		else
		{
			m_window.waitEvent(m_event);
		}
	}
}

void Game::close()
{
	delete m_state;
	m_state = nullptr;

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

	m_titleText.setScale(float(m_currentVideoMode.width) / float(m_bestVideoMode.width),
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

	m_window.draw(m_titleText);

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
