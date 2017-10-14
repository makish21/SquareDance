#include "GameProcessState.h"
#include "Definitions.hpp"


GameProcessState::GameProcessState(Game* game) :
	GameState(game),
	// TODO:
	m_inputHandler(sf::IntRect(0, 0,
							   sf::VideoMode::getDesktopMode().width / 2,
							   sf::VideoMode::getDesktopMode().height),
				   sf::IntRect(sf::VideoMode::getDesktopMode().width / 2, 0,
							   sf::VideoMode::getDesktopMode().width / 2,
							   sf::VideoMode::getDesktopMode().height),
				   sf::Keyboard::Left,
				   sf::Keyboard::Right),
	m_enemySpawner(&m_objects, game->getFileManager()->getSpawnPresets()),
	m_stopwatchText("00:00:00", *game->getFileManager()->getFont("Helvetica"), 50),
	m_state(Begin),
	m_particleSystem(99, game->getWindow()->getSize().x)
{
	m_sound.setBuffer(*game->getFileManager()->getSound("Otis"));
	m_sound.setLoop(true);
	//m_sound.set
	m_sound.setVolume(50.f);
	//m_sound.play();

	m_deathSound.setBuffer(*game->getFileManager()->getSound("RecordRewind"));

	m_stopwatchText.setOrigin(0, m_stopwatchText.getLocalBounds().height / 2);
	m_stopwatchText.setPosition(50.f, 100.f);
	m_stopwatchText.setFillColor(sf::Color::White);

	game->setViewZoom(GAME_VIEW_ZOOM);
	game->getPlayer()->setPosition(GAME_PLAYER_POSITION);
	game->getWorld()->setBoundsColor(GAME_WORLD_COLOR);
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

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			m_game->changeState(new ToMenuTransition(m_game));
			return;
		}
	}
}

void GameProcessState::update(sf::Time elapsed)
{
	m_sessionTime = m_sessionClock.getElapsedTime();

	if (m_sessionTime.asSeconds() < 180.f)
	{
		m_speedFactor = std::sin(m_sessionTime.asSeconds() * PI / 120.f / 2.f) + 1.f;
	}


	switch (m_state)
	{
	case Begin:
		begin(elapsed);
		break;

	case Process:
		process(elapsed);
		break;

	case Revival:
		revival(elapsed);
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

	window.draw(m_particleSystem, m_game->getPlayer()->getTransform());

	window.setView(window.getDefaultView());
	window.draw(m_stopwatchText);
}

void GameProcessState::begin(sf::Time elapsed)
{
	float accelerationFactor;

	if (m_sessionTime.asSeconds() < 0.95f)
	{
		accelerationFactor = std::sin(m_sessionTime.asSeconds() / 1.f * PI / 2.f) * 1.1874027f;

		m_game->getPlayer()->update(*m_game->getWorld(), elapsed * accelerationFactor);
	}
	else
	{
		m_sessionClock.restart();
		m_sound.play();
		m_state = Process;
	}
}

void GameProcessState::process(sf::Time elapsed)
{
	m_stopwatchText.setString(getElapsedString(m_sessionTime.asMilliseconds()));

	m_game->getPlayer()->update(*m_game->getWorld(), elapsed * 1.1874027f/*m_speedFactor*/);

	m_enemySpawner.update(elapsed, m_speedFactor);

	for (auto i = m_objects.begin(); i != m_objects.end();)
	{
		(*i)->update(*m_game->getWorld(), elapsed * m_speedFactor);
		if (m_game->getPlayer()->isAlive())
		{
			if (m_game->getPlayer()->intersects(*(*i)))
			{
				m_sound.stop();
				m_deathSound.play();
				m_sessionClock.restart();
				m_enemySpawner.reset();
				m_particleSystem.setEmitter(m_game->getPlayer()->getPosition());
				m_particleSystem.setDirection(m_game->getPlayer()->getVelocity());
				m_particleSystem.resetParticles();
				m_game->getPlayer()->setScale(0.f, 0.f);
				m_game->getPlayer()->setPosition(400.f, 140.f);
				m_game->getPlayer()->setRotation(45.f);
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

void GameProcessState::revival(sf::Time elapsed)
{
	float elapsedTime = m_sessionTime.asSeconds();

	if (elapsedTime < 3.f)
	{
		float timeFactor = std::sin(elapsedTime / 3.f * PI / 2.f);
		float speedFactor = timeFactor * 4.f + 1.f;

		for (auto i = m_objects.begin(); i != m_objects.end(); i++)
		{
			(*i)->update(*m_game->getWorld(), elapsed * m_speedFactor * speedFactor);
		}
		m_particleSystem.update(elapsed);

		m_game->getPlayer()->setScale(timeFactor, timeFactor);
	}
	else
	{
		m_sessionClock.restart();
		m_state = Begin;
	}
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
