#include "PreparationState.h"
#include "GameProcessState.h"
#include "Definitions.hpp"
#include "ToPauseTransition.h"
#include "RevivalState.h"

PreparationState::PreparationState(Game* const game,
								   const SharedContext& sharedContext) :
	GameState(game,
			  sharedContext)
{
	game->setViewZoom(GAME_VIEW_ZOOM);
	sharedContext.world->setBoundsColor(GAME_WORLD_COLOR);
	sharedContext.player->setScale(1.f, 1.f);
}

PreparationState::~PreparationState()
{
}

void PreparationState::handleInput(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			m_game->changeState(new ToPauseTransition(m_game,
													  m_shared));
			return;
		}
	}
}

void PreparationState::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	if (m_elapsedTime.asSeconds() < PREPARING_DURATION)
	{
		float accelerationFactor = std::sin(m_elapsedTime.asSeconds() / PREPARING_DURATION * PI / 2.f) * 1.1874027f;

		m_shared.player->update(*m_shared.world, elapsed * accelerationFactor);

		m_shared.enemySpawner->update(elapsed, accelerationFactor);

		for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
		{
			(*i)->update(*m_shared.world, elapsed * accelerationFactor);
			if (m_shared.player->isAlive())
			{
				if (m_shared.player->intersects(*(*i)))
				{
					m_shared.enemySpawner->reset();

					m_game->changeState(new RevivalState(m_game,
														 m_shared));
					return;
				}
			}
		}
	}
	else
	{
		if (m_game->hasFocus())
		{
			m_game->changeState(new GameProcessState(m_game,
													 m_shared));
			return;
		}
		else
		{
			m_game->changeState(new ToPauseTransition(m_game,
													  m_shared));
			return;
		}
	}
}

void PreparationState::draw(sf::RenderWindow & window)
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

