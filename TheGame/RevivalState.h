#pragma once
#include "GameState.h"
#include "ParticleSystem.h"

class RevivalState :
	public GameState
{
public:
	RevivalState(Game* const game,
				 FileManager* const fileManager,
				 sf::View* const view,
				 Player* const player,
				 EnemySpawner* const enemySpawner,
				 GameObjects* const gameObjects,
				 World* const world);
	~RevivalState();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow&);

private:
	sf::Time m_elapsed;

	ParticleSystem m_particleSystem;

	sf::Sound m_deathSound;
};

