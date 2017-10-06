#pragma once
#include "GameState.h"
class Transition :
	public GameState
{
public:
	Transition(Game* game,
			   float transitionTime,
			   float newViewZoom,
			   sf::Vector2f newPlayerPosition,
			   sf::Uint8 newWorldAlpha,
			   sf::Uint8 newTitleAlpha);
	~Transition();

protected:
	void calculateTransition(float elapsed);

	float translate(float elapsed, float oldVal, float newVal);

	sf::Clock m_transitionClock;
	float m_transitionTime;

private:
	const float        c_oldViewZoom;
	const sf::Vector2f c_oldPlayerPosition;
	const sf::Uint8    c_oldWorldAlpha;
	const sf::Uint8    c_oldTitleAlpha;

	const float        c_newViewZoom;
	const sf::Vector2f c_newPlayerPosition;
	const sf::Uint8    c_newWorldAlpha;
	const sf::Uint8    c_newTitleAlpha;
};

