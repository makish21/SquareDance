#pragma once
#include "GameState.h"
#include "ParticleSystem.h"

class RevivalState :
	public GameState
{
public:
	RevivalState(Game* const game,
				 const SharedContext& sharedContext);
	~RevivalState();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow&);

private:
	sf::Time m_elapsed;

	ParticleSystem m_particleSystem;

	sf::Sound m_deathSound;
};

