#pragma once
#include "Transition.h"
#include "MainMenuState.h"

class ToMenuTransition :
	public Transition
{
public:
	ToMenuTransition(Game* game);
	~ToMenuTransition();

	virtual void handleInput(sf::Event& event);
	virtual void update(float time);
	virtual void draw(sf::RenderWindow& window);
};

