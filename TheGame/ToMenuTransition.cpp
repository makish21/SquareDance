#include "ToMenuTransition.h"
#include "Definitions.hpp"

ToMenuTransition::ToMenuTransition(Game* const game,
								   const SharedContext& sharedContext) :
	GameState(game,
			  sharedContext),
	c_transitionDuration(sf::seconds(2.f)),
	c_oldPlayerPosition(sharedContext.player->getPosition()),
	c_oldPlayerScale(sharedContext.player->getScale()),
	c_oldViewZoom(game->getViewZoom()),
	c_oldTitleColor(game->getTitleColor()),
	c_oldWorldColor(sharedContext.world->getBoundsColor()),
	c_newViewZoom(MENU_VIEW_ZOOM),
	c_newPlayerPosition(MENU_PLAYER_POSITION),
	c_newPlayerScale(MENU_PLAYER_SCALE),
	c_newWorldColor(MENU_WORLD_COLOR),
	c_newTitleColor(MENU_TITLE_COLOR)
{
}


ToMenuTransition::~ToMenuTransition()
{
	// kill all objects
	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		(*i)->setLife(false);
	}
}

void ToMenuTransition::handleInput(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed ||
		event.type == sf::Event::MouseButtonPressed ||
		event.type == sf::Event::TouchBegan)
	{
		m_game->changeState(new MainMenuState(m_game,
											  m_shared));
		return;
	}
}

void ToMenuTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	m_shared.player->rotate(-10.f * elapsed.asSeconds());

	if (m_elapsedTime <= c_transitionDuration)
	{
		m_game->setViewZoom(transfer(m_elapsedTime, c_transitionDuration, c_oldViewZoom, c_newViewZoom));
		m_shared.player->setPosition(transfer(m_elapsedTime, c_transitionDuration, c_oldPlayerPosition, c_newPlayerPosition));
		m_shared.player->setScale(transfer(m_elapsedTime, c_transitionDuration, c_oldPlayerScale, c_newPlayerScale));
		m_game->setTitleColor(transfer(m_elapsedTime, c_transitionDuration, c_oldTitleColor, c_newTitleColor));
		m_shared.world->setBoundsColor(transfer(m_elapsedTime, c_transitionDuration, c_oldWorldColor, c_newWorldColor));

		float timeFactor = std::sin(m_elapsedTime.asSeconds() / c_transitionDuration.asSeconds() * PI / 2.f);
		float speedFactor = timeFactor * 10.f + 1.f;

		for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
		{
			(*i)->update(*m_shared.world, elapsed * speedFactor);
		}
	}
	else
	{
		m_game->changeState(new MainMenuState(m_game,
											  m_shared));
		return;
	}
}

void ToMenuTransition::draw(sf::RenderWindow & window)
{
	window.draw(*m_shared.background);

	window.setView(*m_shared.gameView);

	window.draw(*m_shared.world);
	window.draw(*m_shared.player);

	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		window.draw(**i);
	}
}
