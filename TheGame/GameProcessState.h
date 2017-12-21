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
					 const SharedContext& sharedContext,
					 sf::Time* const currentTime,
					 sf::Text* const stopwatch,
					 RenderButton* const pauseButton);
	~GameProcessState();

	virtual void clear();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow&);

private:
	sf::Texture m_texture;
	RenderButton* const m_pauseButton;

	sf::Text* const m_stopwatchText;
	sf::Time* const m_currentTime;
	float m_speedFactor;
};

