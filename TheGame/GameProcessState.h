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
	virtual void update(float time);
	virtual void draw(sf::RenderWindow&);

private:
	const float VIEW_ZOOM = 1.f;
	const sf::Vector2f PLAYER_POSITION = sf::Vector2f(400.f, 140.f);
	const sf::Uint8 WORLD_TRANSPARENCY = 10;

	void begin(float time);
	void process(float time);
	void revival(float time);

	State m_state;
	ParticleSystem m_particleSystem;

	std::string getElapsedString(sf::Int32 milliseconds);

	sf::Sound m_sound;

	std::list<Object*> m_objects;

	EnemySpawner m_enemySpawner;

	InputHandler m_inputHandler;

	sf::Text m_stopwatchText;

	sf::Clock m_sessionClock;
	sf::Time m_sessionTime;
	float m_speedFactor;
};

