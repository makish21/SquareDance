#include "ToMenuTransition.h"
#include "Definitions.hpp"

ToMenuTransition::ToMenuTransition(Game* const game,
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
	c_transitionDuration(2.f),
	c_oldPlayerPosition(player->getPosition()),
	c_oldPlayerScale(player->getScale()),
	c_oldViewZoom(game->getViewZoom()),
	c_oldTitleColor(game->getTitleColor()),
	c_oldWorldColor(world->getBoundsColor()),
	c_newViewZoom(MENU_VIEW_ZOOM),
	c_newPlayerPosition(MENU_PLAYER_POSITION),
	c_newPlayerScale(MENU_PLAYER_SCALE),
	c_newWorldColor(MENU_WORLD_COLOR),
	c_newTitleColor(MENU_TITLE_COLOR)
{
}


ToMenuTransition::~ToMenuTransition()
{
}

void ToMenuTransition::handleInput(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed ||
		event.type == sf::Event::MouseButtonPressed ||
		event.type == sf::Event::TouchBegan)
	{
		/*for (auto i = m_objects->begin(); i != m_objects->end(); i++)
		{
			delete (*i);
			(*i) = nullptr;
		}*/

		m_game->changeState(new MainMenuState(m_game,
											  m_fileManager,
											  m_gameView,
											  m_player,
											  m_enemySpawner,
											  m_objects,
											  m_world));
		return;
	}
}

void ToMenuTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	m_player->rotate(-10.f * elapsed.asSeconds());

	if (m_elapsedTime.asSeconds() <= c_transitionDuration)
	{
		m_game->setViewZoom(translate(m_elapsedTime, c_transitionDuration, c_oldViewZoom, c_newViewZoom));
		m_player->setPosition(translate(m_elapsedTime, c_transitionDuration, c_oldPlayerPosition, c_newPlayerPosition));
		m_player->setScale(translate(m_elapsedTime, c_transitionDuration, c_oldPlayerScale, c_newPlayerScale));
		m_game->setTitleColor(translate(m_elapsedTime, c_transitionDuration, c_oldTitleColor, c_newTitleColor));
		m_world->setBoundsColor(translate(m_elapsedTime, c_transitionDuration, c_oldWorldColor, c_newWorldColor));

		float timeFactor = std::sin(m_elapsedTime.asSeconds() / c_transitionDuration * PI / 2.f);
		float speedFactor = timeFactor * 10.f + 1.f;

		for (auto i = m_objects->begin(); i != m_objects->end(); i++)
		{
			(*i)->update(*m_world, elapsed * speedFactor);
		}
	}
	else
	{
		m_game->changeState(new MainMenuState(m_game,
											  m_fileManager,
											  m_gameView,
											  m_player,
											  m_enemySpawner,
											  m_objects,
											  m_world));
		return;
	}
}

void ToMenuTransition::draw(sf::RenderWindow & window)
{
}
