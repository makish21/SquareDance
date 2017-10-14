#include "ToMenuTransition.h"
#include "Definitions.hpp"

ToMenuTransition::ToMenuTransition(Game* game) :
	GameState(game),
	c_transitionDuration(2.f),
	c_oldPlayerPosition(game->getPlayer()->getPosition()),
	c_oldViewZoom(game->getViewZoom()),
	c_oldTitleColor(game->getTitleColor()),
	c_oldWorldColor(game->getWorld()->getBoundsColor()),
	c_newViewZoom(MENU_VIEW_ZOOM),
	c_newPlayerPosition(MENU_PLAYER_POSITION),
	c_newWorldColor(MENU_WORLD_COLOR),
	c_newTitleColor(MENU_TITLE_COLOR)
{
}


ToMenuTransition::~ToMenuTransition()
{
}

void ToMenuTransition::handleInput(sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed ||
		event.type == sf::Event::MouseButtonPressed ||
		event.type == sf::Event::TouchBegan)
	{
		m_game->changeState(new MainMenuState(m_game));
		return;
	}
}

void ToMenuTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	m_game->getPlayer()->rotate(-10.f * elapsed.asSeconds());

	if (m_elapsedTime.asSeconds() <= c_transitionDuration)
	{
		m_game->setViewZoom(translate(m_elapsedTime, c_transitionDuration, c_oldViewZoom, c_newViewZoom));
		m_game->getPlayer()->setPosition(translate(m_elapsedTime, c_transitionDuration, c_oldPlayerPosition, c_newPlayerPosition));
		m_game->setTitleColor(translate(m_elapsedTime, c_transitionDuration, c_oldTitleColor, c_newTitleColor));
		m_game->getWorld()->setBoundsColor(translate(m_elapsedTime, c_transitionDuration, c_oldWorldColor, c_newWorldColor));
	}
	else
	{
		m_game->changeState(new MainMenuState(m_game));
		return;
	}
}

void ToMenuTransition::draw(sf::RenderWindow & window)
{
}
