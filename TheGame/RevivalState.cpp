#include "RevivalState.h"
#include "Definitions.hpp"
#include "PreparationState.h"
#include "PauseState.h"
#include "Animations.h"
#include "ToMenuTransition.h"
#include "Platform.h"

RevivalState::RevivalState(Game* const game,
						   const SharedContext& sharedContext,
						   sf::Time* const currentTime,
						   sf::Text* const stopwatch,
						   RenderButton* const pauseButton) :
	GameState(game,
			  sharedContext),
	m_particleSystem(new ParticleSystem(100,
										sharedContext.player->getColor(),
										sf::seconds(PLAYER_REVIVAL_DURATION - 0.5f))),
	m_stateTime(sf::milliseconds(0)),
	m_deathSound(*sharedContext.fileManager->getSound("RecordRewind")),
	m_currentTime(currentTime),
	m_stopwatchText(stopwatch),
	m_pauseButton(pauseButton)
{
	m_particleSystem->setEmitter(sharedContext.player->getPosition());
	m_particleSystem->setDirection(sharedContext.player->getVelocity());
	m_particleSystem->resetParticles();

	sharedContext.enemySpawner->reset();
	m_deathSound.play();

	Platform::instance().vibrate(sf::milliseconds(100));

	sharedContext.player->setRotation(225.f);
	sharedContext.player->setScale(0.f, 0.f);
	sharedContext.player->setPosition(400.f, 140.f);

	if (*currentTime > *m_shared.bestTime)
	{
		m_shared.fileManager->saveHighScore(*currentTime);
		*m_shared.bestTime = *currentTime;
	}
}

RevivalState::~RevivalState()
{
	delete m_pauseButton;
}

void RevivalState::clear()
{
	delete m_currentTime;
	delete m_stopwatchText;
	delete m_particleSystem;
}

void RevivalState::handleInput(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			m_game->changeState(new ToMenuTransition(m_game,
													 m_shared,
													 m_currentTime,
													 m_stopwatchText,
													 m_particleSystem));
		}
	}
}

void RevivalState::update(sf::Time elapsed)
{
	m_stateTime += elapsed;
	float elapsedTime = m_stateTime.asSeconds();

	if (elapsedTime < PLAYER_REVIVAL_DURATION)
	{
		float timeFactor = std::sin(elapsedTime / PLAYER_REVIVAL_DURATION * PI / 2.f);
		float speedFactor = timeFactor * 4.f + (std::sin(m_currentTime->asSeconds() * PI / 120.f / 2.f) + 1.f);

		for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
		{
			(*i)->update(*m_shared.world, elapsed * speedFactor);
		}
		m_particleSystem->update(elapsed);

		m_shared.player->setScale(timeFactor, timeFactor);
		m_shared.player->setRotation(transfer(m_stateTime,
											  sf::seconds(PLAYER_REVIVAL_DURATION),
											  225.f, 45.f));
	}
	else
	{
		*m_currentTime = sf::Time::Zero;
		m_stopwatchText->setString("0.00");
		m_game->changeState(new PreparationState(m_game,
												 m_shared,
												 m_currentTime,
												 m_stopwatchText));
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

	window.draw(*m_particleSystem);

	window.setView(window.getDefaultView());
	window.draw(*m_pauseButton);
	window.draw(*m_stopwatchText);
}
