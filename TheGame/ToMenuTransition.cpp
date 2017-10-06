#include "ToMenuTransition.h"

#define VIEW_ZOOM 0.5f
#define PLAYER_POSITION sf::Vector2f(400, 140)
#define WORLD_ALPHA 0
#define TITLE_ALPHA 255

ToMenuTransition::ToMenuTransition(Game* game) :
	Transition(game,
			   2.f,
			   VIEW_ZOOM,
			   PLAYER_POSITION,
			   WORLD_ALPHA,
			   TITLE_ALPHA)
{
}


ToMenuTransition::~ToMenuTransition()
{
}

void ToMenuTransition::handleInput(sf::Event & event)
{
	while (m_game->pollEvent(event))
	{
		m_game->handleEvent(event);

		if (event.type == sf::Event::KeyPressed ||
			event.type == sf::Event::MouseButtonPressed ||
			event.type == sf::Event::TouchBegan)
		{
			m_game->changeState(new MainMenuState(m_game));
			return;
		}
	}
}

void ToMenuTransition::update(float time)
{
	m_game->getPlayer()->rotate(-0.2f);

	sf::Time elapsed = m_transitionClock.getElapsedTime();

	if (elapsed.asSeconds() <= m_transitionTime)
	{
		calculateTransition(elapsed.asSeconds());
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
