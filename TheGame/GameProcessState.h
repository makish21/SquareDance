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
#include "RenderButton.h"

class GameProcessState :
	public GameState
{
public:
	GameProcessState(Game* const game,
					 const SharedContext& sharedContext);
	~GameProcessState();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow&);

private:
	std::string getElapsedString(sf::Int32 milliseconds);

	sf::Text m_stopwatchText;
	sf::Texture m_texture;
	RenderButton m_pauseButton;

	//sf::Clock m_sessionClock;
	sf::Time m_sessionTime;
	float m_speedFactor;
};

