#include "Transition.h"
#include "Definitions.hpp"

Transition::Transition(Game* game,
					   float transitionTime,
					   float newViewZoom,
					   sf::Vector2f newPlayerPosition,
					   sf::Uint8 newWorldAlpha,
					   sf::Uint8 newTitleAlpha) :
	GameState(game),
	m_transitionTime(transitionTime),
	c_oldPlayerPosition(game->getPlayer()->getPosition()),
	c_oldViewZoom(game->getViewZoom()),
	c_oldTitleAlpha(game->getTitleAlpha()),
	c_oldWorldAlpha(game->getWorld()->getAlpha()),
	c_newViewZoom(newViewZoom),
	c_newPlayerPosition(newPlayerPosition),
	c_newWorldAlpha(newWorldAlpha),
	c_newTitleAlpha(newTitleAlpha)
{
}

Transition::~Transition()
{
}

void Transition::calculateTransition(float elapsed)
{
	float factor = 1.f - std::sin(elapsed / m_transitionTime * PI / 2.f);

	m_game->setViewZoom((c_oldViewZoom - c_newViewZoom) * factor + c_newViewZoom);
	m_game->getPlayer()->setPosition((c_oldPlayerPosition - c_newPlayerPosition) * factor + c_newPlayerPosition);
	m_game->getWorld()->setAlpha(static_cast<sf::Uint8>((c_oldWorldAlpha - c_newWorldAlpha) * factor + c_newWorldAlpha));
	m_game->setTitleAlpha(static_cast<sf::Uint8>((c_oldTitleAlpha - c_newTitleAlpha) * factor + c_newTitleAlpha));
}

float Transition::translate(float elapsed, float oldVal, float newVal)
{
	float factor = 1.f - std::sin(elapsed / m_transitionTime * PI / 2.f);

	return (oldVal - newVal) * factor + newVal;
}
