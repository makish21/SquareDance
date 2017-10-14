#pragma once
#include "Transition.h"
#include "GameProcessState.h"

class ToGameTransition :
	public GameState
{
public:
	ToGameTransition(Game* game);
	~ToGameTransition();

	virtual void handleInput(sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow& window);

private:
	sf::Time m_elapsedTime;
	const float c_transitionDuration;

	const float c_oldPlayerRotation;
	const float c_newPlayerRotation;

	const float        c_oldViewZoom;
	const sf::Vector2f c_oldPlayerPosition;
	const sf::Color    c_oldWorldColor;
	const sf::Color    c_oldTitleColor;

	const float        c_newViewZoom;
	const sf::Vector2f c_newPlayerPosition;
	const sf::Color    c_newWorldColor;
	const sf::Color    c_newTitleColor;
};

