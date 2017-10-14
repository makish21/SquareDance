#include "Transition.h"
#include "Definitions.hpp"

//Transition::Transition(Game* game,
//					   float transitionTime,
//					   float newViewZoom,
//					   sf::Vector2f newPlayerPosition,
//					   sf::Color newWorldColor,
//					   sf::Color newTitleColor) :
//	GameState(game),
//	m_transitionTime(transitionTime),
//	c_oldPlayerPosition(game->getPlayer()->getPosition()),
//	c_oldViewZoom(game->getViewZoom()),
//	c_oldTitleColor(game->getTitleColor()),
//	c_oldWorldColor(game->getWorld()->getBoundsColor()),
//	c_newViewZoom(newViewZoom),
//	c_newPlayerPosition(newPlayerPosition),
//	c_newWorldColor(newWorldColor),
//	c_newTitleColor(newTitleColor)
//{
//}
//
//Transition::~Transition()
//{
//}
//
//void Transition::calculateTransition(sf::Time elapsed)
//{
//	m_game->setViewZoom(translate(elapsed, m_transitionTime, c_oldViewZoom, c_newViewZoom));
//	m_game->getPlayer()->setPosition(translate(elapsed, m_transitionTime, c_oldPlayerPosition, c_newPlayerPosition));
//	m_game->setTitleColor(translate(elapsed, m_transitionTime, c_oldTitleColor, c_newTitleColor));
//	m_game->getWorld()->setBoundsColor(translate(elapsed, m_transitionTime, c_oldWorldColor, c_newWorldColor));
//}

float translate(sf::Time elapsed, float duration, float oldVal, float newVal)
{
	float factor = 1.f - std::sin(elapsed.asSeconds() / duration * PI / 2.f);

	return (oldVal - newVal) * factor + newVal;
}

sf::Vector2f translate(sf::Time elapsed, float duration, sf::Vector2f oldVal, sf::Vector2f newVal)
{
	float factor = 1.f - std::sin(elapsed.asSeconds() / duration * PI / 2.f);

	return (oldVal - newVal) * factor + newVal;
}

sf::Color translate(sf::Time elapsed, float duration, sf::Color oldVal, sf::Color newVal)
{
	float factor = 1.f - std::sin(elapsed.asSeconds() / duration * PI / 2.f);

	sf::Color color;

	color.r = (oldVal.r - newVal.r) * factor + newVal.r;
	color.g = (oldVal.g - newVal.g) * factor + newVal.g;
	color.b = (oldVal.b - newVal.b) * factor + newVal.b;
	color.a = (oldVal.a - newVal.a) * factor + newVal.a;

	return color;
}
