#pragma once
#include <SFML\Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include "InputHandler.h"
#include "EnemySpawner.h"
#include "Definitions.hpp"

class GameState
{
public:
	GameState(Game* const game,
			  FileManager* const fileManager,
			  sf::View* const view,
			  Player* const player,
			  EnemySpawner* const enemySpawner,
			  GameObjects* const gameObjects,
			  World* const world) :
		m_game(game),
		m_fileManager(fileManager),
		m_gameView(view),
		m_player(player),
		m_objects(gameObjects),
		m_enemySpawner(enemySpawner),
		m_world(world)
	{
	}
	virtual ~GameState()
	{
	};


	virtual void handleInput(const sf::Event& event) = 0;
	virtual void update(sf::Time elapsed) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
protected:
	Game*          const m_game;

	FileManager* const m_fileManager;

	sf::View*      const m_gameView;
	GameObjects*   const m_objects;
	Player*        const m_player;
	EnemySpawner*  const m_enemySpawner;
	World*         const m_world;
};

