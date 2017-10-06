#include "GameProcessState.h"
#include "Definitions.hpp"


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
	m_enemySpawner(&m_objects, game->getFileManager()->getSpawnPresets()),
	m_stopwatchText("0", *game->getFileManager()->getFont("Helvetica"), 50),
	m_state(Begin),
	m_particleSystem(99)
{
	m_sound.setBuffer(*game->getFileManager()->getSound("Otis"));
	m_sound.setLoop(true);
	//m_sound.set
	m_sound.setVolume(50.f);
	//m_sound.play();

	m_stopwatchText.setOrigin(0, m_stopwatchText.getLocalBounds().height / 2);
	m_stopwatchText.setPosition(50.f, 100.f);
	m_stopwatchText.setFillColor(sf::Color::White);

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

	while (m_game->pollEvent(event))
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
		m_speedFactor = std::sin(m_sessionTime.asSeconds() * PI / 120.f / 2.f) + 1.f;
	}
	
	m_stopwatchText.setString(getElapsedString(m_sessionTime.asMilliseconds()));

	switch (m_state)
	{
	case Begin:
		begin(time);
		break;

	case Process:
		process(time);
		break;

	case Revival:
		revival(time);
		break;
	default:
		break;
	}
}

void GameProcessState::draw(sf::RenderWindow &window)
{
	window.setView(*m_game->getView());

	for (auto i = m_objects.begin(); i != m_objects.end(); i++)
	{
		window.draw(**i);
	}

	window.draw(m_particleSystem);

	window.setView(window.getDefaultView());
	window.draw(m_stopwatchText);
}

void GameProcessState::begin(float time)
{
	float accelerationFactor;

	if (m_sessionTime.asSeconds() < 1.f)
	{
		accelerationFactor = std::sin(m_sessionTime.asSeconds() / 1.f * PI / 2.f);

		m_game->getPlayer()->update(*m_game->getWorld(), accelerationFactor);
	}
	else
	{
		m_sound.play();
		m_state = Process;
	}
}

void GameProcessState::process(float time)
{
	m_game->getPlayer()->update(*m_game->getWorld(), time * 1.1874027f/*m_speedFactor*/);

	m_enemySpawner.update(m_speedFactor);

	for (auto i = m_objects.begin(); i != m_objects.end();)
	{
		(*i)->update(*m_game->getWorld(), time * m_speedFactor);
		if (m_game->getPlayer()->isAlive())
		{
			if (m_game->getPlayer()->intersects(*(*i)))
			{
				m_sound.stop();
				m_sessionClock.restart();
				m_particleSystem.setEmitter(m_game->getPlayer()->getPosition());
				m_particleSystem.setDirection(m_game->getPlayer()->getVelocity());
				m_particleSystem.resetParticles();
				m_state = Revival;
				//m_game->changeState(new ToMenuTransition(m_game));
				//return;
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

void GameProcessState::revival(float time)
{
	m_particleSystem.update(time);
}

std::string GameProcessState::getElapsedString(sf::Int32 milliseconds)
{
	int ms = milliseconds % 1000 / 10;
	int seconds = (milliseconds / 1000) % 60;
	int minutes = (milliseconds / (1000 * 60)) % 60;
	int hours = (milliseconds / (1000 * 60 * 60)) % 24;

	std::string msString = (ms < 10 ? "0" : "") + std::to_string(ms);
	std::string secString = (seconds < 10 ? "0" : "") + std::to_string(seconds);
	std::string minString = (minutes < 10 ? "0" : "") + std::to_string(minutes);
	std::string hString = (hours < 10 ? "0" : "") + std::to_string(hours);

	return std::string(hours > 0 ? hString + ':' : "" + 
					   minString + ':' + 
					   secString + ':' + 
					   msString);
}
