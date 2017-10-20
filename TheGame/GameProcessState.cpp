#include "GameProcessState.h"
#include "Definitions.hpp"
#include "RevivalState.h"

GameProcessState::GameProcessState(Game* const game,
								   FileManager* const fileManager,
								   sf::View* const view,
								   Player* const player,
								   EnemySpawner* const enemySpawner,
								   GameObjects* const gameObjects,
								   World* const world) :
	GameState(game,
			  fileManager,
			  view,
			  player,
			  enemySpawner,
			  gameObjects,
			  world),	
	m_stopwatchText("00:00:00", *m_fileManager->getFont("Helvetica"), 50)
{
	m_stopwatchText.setOrigin(0, m_stopwatchText.getLocalBounds().height / 2);
	m_stopwatchText.setPosition(50.f, 100.f);
	m_stopwatchText.setFillColor(sf::Color::White);
}


GameProcessState::~GameProcessState()
{
}

void GameProcessState::handleInput(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			m_game->changeState(new ToMenuTransition(m_game,
													 m_fileManager,
													 m_gameView,
													 m_player,
													 m_enemySpawner,
													 m_objects,
													 m_world));
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

	m_stopwatchText.setString(getElapsedString(m_sessionTime.asMilliseconds()));

	m_player->update(*m_world, elapsed * 1.1874027f/*m_speedFactor*/);

	m_enemySpawner->update(elapsed, m_speedFactor);

	for (auto i = m_objects->begin(); i != m_objects->end();)
	{
		(*i)->update(*m_world, elapsed * m_speedFactor);
		if (m_player->isAlive())
		{
			if (m_player->intersects(*(*i)))
			{
				//m_sound.stop();
				m_sessionClock.restart();
				m_enemySpawner->reset();

				m_game->changeState(new RevivalState(m_game,
													 m_fileManager,
													 m_gameView,
													 m_player,
													 m_enemySpawner,
													 m_objects,
													 m_world));
				return;
			}
		}
		if (!(*i)->isAlive())
		{
			delete (*i);
			(*i) = nullptr;

			i = m_objects->erase(i);
		}
		else
		{
			i++;
		}
	}
}

void GameProcessState::draw(sf::RenderWindow &window)
{
	window.setView(window.getDefaultView());
	window.draw(m_stopwatchText);
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
