#pragma once
#include "GameState.h"
class PreparationState :
	public GameState
{
public:
	PreparationState(Game* const game,
					 FileManager* const fileManager,
					 sf::View* const view,
					 Player* const player,
					 EnemySpawner* const enemySpawner,
					 GameObjects* const gameObjects,
					 World* const world);
	~PreparationState();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow&);

private:
	sf::Time m_elapsed;
};

