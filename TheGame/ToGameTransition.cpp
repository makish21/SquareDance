#include "ToGameTransition.h"
#include "Definitions.hpp"
#include "PreparationState.h"

ToGameTransition::ToGameTransition(Game* const game,
								   const SharedContext& sharedContext) :
	GameState(game,
			  sharedContext),
	c_transitionDuration(sf::seconds(TO_GAME_TRANSITION_DURATION)),
	c_oldPlayerPosition(sharedContext.player->getPosition()),
	c_oldViewZoom(game->getViewZoom()),
	c_oldTitleColor(game->getTitleColor()),
	c_oldWorldColor(sharedContext.world->getBoundsColor()),
	c_newViewZoom(GAME_VIEW_ZOOM),
	c_newPlayerPosition(GAME_PLAYER_POSITION),
	c_newWorldColor(GAME_WORLD_COLOR),
	c_newTitleColor(GAME_TITLE_COLOR),
	c_oldPlayerRotation(sharedContext.player->getRotation()),
	c_newPlayerRotation(c_oldPlayerRotation - (std::fmod(c_oldPlayerRotation, 90.f)) + 45.f)
{
}

ToGameTransition::~ToGameTransition()
{
}

void ToGameTransition::handleInput(const sf::Event & event)
{
}

void ToGameTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	if (m_elapsedTime <= c_transitionDuration)
	{
		m_game->setViewZoom(transfer(m_elapsedTime, c_transitionDuration, c_oldViewZoom, c_newViewZoom));
		m_shared.player->setPosition(transfer(m_elapsedTime, c_transitionDuration, c_oldPlayerPosition, c_newPlayerPosition));
		m_game->setTitleColor(transfer(m_elapsedTime, c_transitionDuration, c_oldTitleColor, c_newTitleColor));
		m_shared.world->setBoundsColor(transfer(m_elapsedTime, c_transitionDuration, c_oldWorldColor, c_newWorldColor));

		m_shared.player->setRotation(transfer(m_elapsedTime,
											   c_transitionDuration,
											   c_oldPlayerRotation,
											   c_newPlayerRotation));
	}
	else
	{
		m_game->changeState(new PreparationState(m_game,
												 m_shared));
		return;
	}
}

void ToGameTransition::draw(sf::RenderWindow & window)
{
	window.draw(*m_shared.background);

	window.setView(*m_shared.gameView);

	window.draw(*m_shared.world);
	window.draw(*m_shared.player);
}
