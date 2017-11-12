#include "RevivalState.h"
#include "Definitions.hpp"
#include "PreparationState.h"
#include "PauseState.h"
#include "Android.hpp"
#include "Animations.h"

RevivalState::RevivalState(Game* const game,
						   const SharedContext& sharedContext) :
	GameState(game,
			  sharedContext),
	m_particleSystem(100, sharedContext.player->getColor(), sf::seconds(PLAYER_REVIVAL_DURATION - 0.5f)),
	m_elapsed(sf::milliseconds(0)),
	m_deathSound(*sharedContext.fileManager->getSound("RecordRewind"))
{
	m_particleSystem.setEmitter(sharedContext.player->getPosition());
	m_particleSystem.setDirection(sharedContext.player->getVelocity());
	m_particleSystem.resetParticles();

	sharedContext.enemySpawner->reset();
	m_deathSound.play();
#if defined(__ANDROID__)
	vibrate(sf::milliseconds(100));
#endif

	sharedContext.player->setRotation(225.f);
	sharedContext.player->setScale(0.f, 0.f);
	sharedContext.player->setPosition(400.f, 140.f);
}

RevivalState::~RevivalState()
{
}

void RevivalState::handleInput(const sf::Event & event)
{
}

void RevivalState::update(sf::Time elapsed)
{
	m_elapsed += elapsed;
	float elapsedTime = m_elapsed.asSeconds();

	if (elapsedTime < PLAYER_REVIVAL_DURATION)
	{
		float timeFactor = std::sin(elapsedTime / PLAYER_REVIVAL_DURATION * PI / 2.f);
		float speedFactor = timeFactor * 4.f + 1.f;

		for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
		{
			(*i)->update(*m_shared.world, elapsed * speedFactor);
		}
		m_particleSystem.update(elapsed);

		m_shared.player->setScale(timeFactor, timeFactor);
		m_shared.player->setRotation(transfer(m_elapsed,
											  sf::seconds(PLAYER_REVIVAL_DURATION),
											  225.f, 45.f));
	}
	else
	{
		m_game->changeState(new PreparationState(m_game,
												 m_shared));
		return;
	}
}

void RevivalState::draw(sf::RenderWindow & window)
{
	window.draw(*m_shared.background);

	window.setView(*m_shared.gameView);

	window.draw(*m_shared.world);
	window.draw(*m_shared.player);

	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		window.draw(**i);
	}

	window.draw(m_particleSystem);
}
