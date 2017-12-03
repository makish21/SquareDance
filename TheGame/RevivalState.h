#pragma once
#include "GameState.h"
#include "ParticleSystem.h"

class RevivalState :
	public GameState
{
public:
	RevivalState(Game* const game,
				 const SharedContext& sharedContext,
				 sf::Time* const currentTime,
				 sf::Text* const stopwatch);
	~RevivalState();

	virtual void clear();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow&);

private:
	sf::Time m_stateTime;

	ParticleSystem m_particleSystem;

	sf::Time* const m_currentTime;
	sf::Text* const m_stopwatchText;

	sf::Sound m_deathSound;
};

