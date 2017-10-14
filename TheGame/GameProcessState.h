#pragma once
#include <list>
#include <fstream>
#include <sstream>

#include <SFML\Audio.hpp>

#include "GameState.h"
#include "ToMenuTransition.h"
#include "Object.hpp"
#include "DefaultEnemy.h"
#include "InputHandler.h"
#include "EnemySpawner.h"
#include "ParticleSystem.h"

enum State
{
	Begin,
	Process,
	Revival
};

class GameProcessState :
	public GameState
{
public:
	GameProcessState(Game* game);
	~GameProcessState();

	virtual void handleInput(sf::Event&);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow&);

private:
	const sf::Uint8 WORLD_TRANSPARENCY = 10;

	void begin(sf::Time elapsed);
	void process(sf::Time elapsed);
	void revival(sf::Time elapsed);

	State m_state;
	ParticleSystem m_particleSystem;

	std::string getElapsedString(sf::Int32 milliseconds);

	sf::Sound m_sound;
	sf::Sound m_deathSound;

	std::list<Object*> m_objects;

	EnemySpawner m_enemySpawner;

	InputHandler m_inputHandler;

	sf::Text m_stopwatchText;

	sf::Clock m_sessionClock;
	sf::Time m_sessionTime;
	float m_speedFactor;
};

