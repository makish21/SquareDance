#include "ToGameTransition.h"
#include "Definitions.hpp"

ToGameTransition::ToGameTransition(Game* game) :
	GameState(game),
	c_transitionDuration(2.f),
	c_oldPlayerPosition(game->getPlayer()->getPosition()),
	c_oldViewZoom(game->getViewZoom()),
	c_oldTitleColor(game->getTitleColor()),
	c_oldWorldColor(game->getWorld()->getBoundsColor()),
	c_newViewZoom(GAME_VIEW_ZOOM),
	c_newPlayerPosition(GAME_PLAYER_POSITION),
	c_newWorldColor(GAME_WORLD_COLOR),
	c_newTitleColor(GAME_TITLE_COLOR),
	c_oldPlayerRotation(game->getPlayer()->getRotation()),
	c_newPlayerRotation(c_oldPlayerRotation - (std::fmod(c_oldPlayerRotation, 90.f)) + 45.f)
{
}

ToGameTransition::~ToGameTransition()
{
}

void ToGameTransition::handleInput(sf::Event & event)
{
}

void ToGameTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	if (m_elapsedTime.asSeconds() <= c_transitionDuration)
	{
		m_game->setViewZoom(translate(m_elapsedTime, c_transitionDuration, c_oldViewZoom, c_newViewZoom));
		m_game->getPlayer()->setPosition(translate(m_elapsedTime, c_transitionDuration, c_oldPlayerPosition, c_newPlayerPosition));
		m_game->setTitleColor(translate(m_elapsedTime, c_transitionDuration, c_oldTitleColor, c_newTitleColor));
		m_game->getWorld()->setBoundsColor(translate(m_elapsedTime, c_transitionDuration, c_oldWorldColor, c_newWorldColor));

		m_game->getPlayer()->setRotation(translate(m_elapsedTime,
												   c_transitionDuration,
												   c_oldPlayerRotation,
												   c_newPlayerRotation));
	}
	else
	{
		m_game->changeState(new GameProcessState(m_game));
		return;
	}
}

void ToGameTransition::draw(sf::RenderWindow & window)
{
}
