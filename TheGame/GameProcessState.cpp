#include "GameProcessState.h"



GameProcessState::GameProcessState(Game* game) :
	GameState(game),
#ifdef __ANDROID__
	m_inputHandler(sf::IntRect(0, 0,
							   sf::VideoMode::getDesktopMode().width / 2,
							   sf::VideoMode::getDesktopMode().height),
				   sf::IntRect(sf::VideoMode::getDesktopMode().width / 2, 0,
							   sf::VideoMode::getDesktopMode().width / 2,
							   sf::VideoMode::getDesktopMode().height)),
#else
	m_inputHandler(sf::Keyboard::Left,
				   sf::Keyboard::Right),
#endif // __ANDROID__
	m_spawnPresets("SpawnPresets.txt"),
	m_enemySpawner(&m_objects, &m_spawnPresets)
{
	m_sound.setBuffer(*game->getSoundBuffer("Otis"));
	m_sound.setLoop(true);
	m_sound.setPitch(1.f);
	m_sound.setVolume(50.f);
	m_sound.play();

	game->setViewZoom(VIEW_ZOOM);
	game->getPlayer()->setPosition(PLAYER_POSITION);
	game->getWorld()->setAlpha(WORLD_TRANSPARENCY);
}


GameProcessState::~GameProcessState()
{
	m_sound.stop();
}

void GameProcessState::handleInput(sf::Event &event)
{
	Command* command = m_inputHandler.handleInput(event);

	if (command != nullptr)
	{
		command->execute(*m_game->getPlayer());
	}

	while (m_game->getWindow()->pollEvent(event))
	{
		m_game->handleEvent(event);

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_game->changeState(new ToMenuTransition(m_game));
				return;
			}
		}
	}
}

void GameProcessState::update(float time)
{
	m_sessionTime = m_sessionClock.getElapsedTime();

	if (m_sessionTime.asSeconds() < 180.f)
	{
		m_speedFactor = std::sin(m_sessionTime.asSeconds() * 3.1415f / 120.f / 2.f) + 1.f;
	}

	m_enemySpawner.update(m_speedFactor);

	m_game->getPlayer()->update(*m_game->getWorld(), time * m_speedFactor);

	for (auto i = m_objects.begin(); i != m_objects.end();)
	{
		(*i)->update(*m_game->getWorld(), time * m_speedFactor);
		if (m_game->getPlayer()->isAlive())
		{
			if (m_game->getPlayer()->intersects(*(*i)))
			{
				m_game->changeState(new ToMenuTransition(m_game));
				return;
			}
		}
		if (!(*i)->isAlive())
		{
			delete (*i);
			(*i) = nullptr;

			i = m_objects.erase(i);
		}
		else
		{
			i++;
		}
	}
}

void GameProcessState::draw(sf::RenderWindow &window)
{
	window.setView(*m_game->getView());

	for (auto i = m_objects.begin(); i != m_objects.end(); i++)
	{
		window.draw(**i);
	}
}