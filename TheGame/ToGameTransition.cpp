#include "ToGameTransition.h"
#include "Definitions.hpp"
#include "PreparationState.h"

ToGameTransition::ToGameTransition(Game* const game,
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
	c_transitionDuration(TO_GAME_TRANSITION_DURATION),
	c_oldPlayerPosition(player->getPosition()),
	c_oldViewZoom(game->getViewZoom()),
	c_oldTitleColor(game->getTitleColor()),
	c_oldWorldColor(world->getBoundsColor()),
	c_newViewZoom(GAME_VIEW_ZOOM),
	c_newPlayerPosition(GAME_PLAYER_POSITION),
	c_newWorldColor(GAME_WORLD_COLOR),
	c_newTitleColor(GAME_TITLE_COLOR),
	c_oldPlayerRotation(player->getRotation()),
	c_newPlayerRotation(c_oldPlayerRotation - (std::fmod(c_oldPlayerRotation, 90.f)) + 45.f)
{
}

ToGameTransition::~ToGameTransition()
{
}

void ToGameTransition::handleInput(const sf::Event & event)
{
}

void ToGameTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	if (m_elapsedTime.asSeconds() <= c_transitionDuration)
	{
		m_game->setViewZoom(translate(m_elapsedTime, c_transitionDuration, c_oldViewZoom, c_newViewZoom));
		m_player->setPosition(translate(m_elapsedTime, c_transitionDuration, c_oldPlayerPosition, c_newPlayerPosition));
		m_game->setTitleColor(translate(m_elapsedTime, c_transitionDuration, c_oldTitleColor, c_newTitleColor));
		m_world->setBoundsColor(translate(m_elapsedTime, c_transitionDuration, c_oldWorldColor, c_newWorldColor));

		m_player->setRotation(translate(m_elapsedTime,
												   c_transitionDuration,
												   c_oldPlayerRotation,
												   c_newPlayerRotation));
	}
	else
	{
		m_game->changeState(new PreparationState(m_game,
												 m_fileManager,
												 m_gameView,
												 m_player,
												 m_enemySpawner,
												 m_objects,
												 m_world));
		return;
	}
}

void ToGameTransition::draw(sf::RenderWindow & window)
{
}
