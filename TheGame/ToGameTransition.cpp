#include "ToGameTransition.h"

#define VIEW_ZOOM 1.f
#define PLAYER_POSITION sf::Vector2f(400.f, 140.f)
#define WORLD_ALPHA 10
#define TITLE_ALPHA 0

ToGameTransition::ToGameTransition(Game* game) :
	Transition(game,
			   2.f,
			   VIEW_ZOOM,
			   PLAYER_POSITION,
			   WORLD_ALPHA,
			   TITLE_ALPHA),
	c_oldPlayerRotation(game->getPlayer()->getRotation()),
	c_newPlayerRotation(c_oldPlayerRotation - (std::fmod(c_oldPlayerRotation, 90.f)) + 45.f)
{
}

ToGameTransition::~ToGameTransition()
{
}

void ToGameTransition::handleInput(sf::Event & event)
{
	while (m_game->pollEvent(event))
	{
		m_game->handleEvent(event);
	}
}

void ToGameTransition::update(float time)
{
	sf::Time elapsed = m_transitionClock.getElapsedTime();

	if (elapsed.asSeconds() <= m_transitionTime)
	{
		calculateTransition(elapsed.asSeconds());
		m_game->getPlayer()->setRotation(translate(elapsed.asSeconds(),
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
