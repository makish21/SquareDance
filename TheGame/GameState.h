#pragma once
#include <SFML\Graphics.hpp>
#include "Game.h"

class GameState
{
public:
	GameState(Game* game) :
		m_game(game)
	{
	}
	virtual ~GameState()
	{
	};


	virtual void handleInput(sf::Event&) = 0;
	virtual void update(sf::Time elapsed) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
protected:
	Game* m_game;
};

