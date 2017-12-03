#pragma once
#include <SFML\Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include "InputHandler.h"
#include "EnemySpawner.h"
#include "Definitions.hpp"

class GameState : 
	public Subject
{
public:
	GameState(Game* const game,
			  const SharedContext& sharedContext) :
		m_game(game),
		m_shared(sharedContext)
	{
	}
	virtual ~GameState()
	{
	};

	virtual void clear() = 0;

	virtual void handleInput(const sf::Event& event) = 0;
	virtual void update(sf::Time elapsed) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
protected:
	Game* const m_game;

	SharedContext m_shared;
};

