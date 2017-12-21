#pragma once
#include "GameState.h"
#include "ParticleSystem.h"
#include "RenderButton.h"

class RevivalState :
	public GameState
{
public:
	RevivalState(Game* const game,
				 const SharedContext& sharedContext,
				 sf::Time* const currentTime,
				 sf::Text* const stopwatch,
				 RenderButton* const pauseButton);
	~RevivalState();

	virtual void clear();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow&);

private:
	sf::Time m_stateTime;

	sf::Time* const m_currentTime;
	sf::Text* const m_stopwatchText;

	ParticleSystem* const m_particleSystem;

	RenderButton* const m_pauseButton;

	sf::Sound m_deathSound;
};

