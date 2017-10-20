#include "PreparationState.h"
#include "GameProcessState.h"
#include "Definitions.hpp"
#include "MainMenuState.h"
#include "RevivalState.h"

PreparationState::PreparationState(Game* const game,
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
			  world)
{
	game->setViewZoom(GAME_VIEW_ZOOM);
	player->setPosition(GAME_PLAYER_POSITION);
	world->setBoundsColor(GAME_WORLD_COLOR);

}

PreparationState::~PreparationState()
{
}

void PreparationState::handleInput(const sf::Event & event)
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

void PreparationState::update(sf::Time elapsed)
{
	m_elapsed += elapsed;

	if (m_elapsed.asSeconds() < PREPARING_DURATION)
	{
		float accelerationFactor = std::sin(m_elapsed.asSeconds() / PREPARING_DURATION * PI / 2.f) * 1.1874027f;

		m_player->update(*m_world, elapsed * accelerationFactor);

		for (auto i = m_objects->begin(); i != m_objects->end();)
		{
			(*i)->update(*m_world, elapsed * accelerationFactor);
			if (m_player->isAlive())
			{
				if (m_player->intersects(*(*i)))
				{
					//m_sound.stop();
					//m_sessionClock.restart();
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
	else
	{
		m_game->changeState(new GameProcessState(m_game,
												 m_fileManager,
												 m_gameView,
												 m_player,
												 m_enemySpawner,
												 m_objects,
												 m_world));
		return;
	}
}

void PreparationState::draw(sf::RenderWindow & window)
{

}
