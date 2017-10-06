#pragma once
#include "Transition.h"
#include "GameProcessState.h"

class ToGameTransition :
	public Transition
{
public:
	ToGameTransition(Game* game);
	~ToGameTransition();

	virtual void handleInput(sf::Event& event);
	virtual void update(float time);
	virtual void draw(sf::RenderWindow& window);

private:
	const float c_oldPlayerRotation;
	const float c_newPlayerRotation;
};

