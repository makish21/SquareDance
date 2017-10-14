#pragma once
#include "GameState.h"

float translate(sf::Time elapsed, float duration, float oldVal, float newVal);
sf::Vector2f translate(sf::Time elapsed, float duration, sf::Vector2f oldVal, sf::Vector2f newVal);
sf::Color translate(sf::Time elapsed, float duration, sf::Color oldVal, sf::Color newVal);

//class Transition :
//	public GameState
//{
//public:
//	Transition(Game* game,
//			   float transitionTime,
//			   float newViewZoom,
//			   sf::Vector2f newPlayerPosition,
//			   sf::Color newWorldColor,
//			   sf::Color newTitleColor);
//	~Transition();
//
//protected:
//	void calculateTransition(sf::Time elapsed);
//
//	//float translate(sf::Time elapsed, float oldVal, float newVal);
//
//	sf::Clock m_transitionClock;
//	float m_transitionTime;
//
//private:
//};

