#include "PreparationState.h"
#include "GameProcessState.h"
#include "Definitions.hpp"
#include "ToPauseTransition.h"
#include "RevivalState.h"
#include "Functions.hpp"
#include "Animations.h"

PreparationState::PreparationState(Game* const game,
								   const SharedContext& sharedContext,
								   sf::Time* const currentTime,
								   sf::Text* const stopwatch) :
	GameState(game,
			  sharedContext),
	m_stopwatchText(stopwatch),
	m_currentTime(currentTime),
	m_pauseButton(new RenderButton(sf::FloatRect(game->getCurrentVideoMode().width - 60,
												 0, 60, 60))),
	m_effect(new sf::RenderTexture),
	m_effectTime(new sf::Time(sf::Time::Zero))
{
	game->setViewZoom(GAME_VIEW_ZOOM);
	sharedContext.world->setBoundsColor(GAME_WORLD_COLOR);
	sharedContext.player->setScale(1.f, 1.f);

	m_pauseButton->setTexture(*sharedContext.fileManager->getTexture("PauseIcon"));
	m_pauseButton->setAlignment(Center, Center);
	m_pauseButton->setFitting(Fit, 0.5f);

	sf::Text text(getElapsedString(*m_shared.bestTime),
				  *m_shared.fileManager->getFont("Titles"),
				  m_stopwatchText->getCharacterSize());
	sf::FloatRect rect = text.getLocalBounds();
	m_effect->create(rect.left + rect.width,
					 rect.top + rect.height);
	m_effect->clear(sf::Color::Transparent);
	m_effect->draw(text);
	m_effect->display();
}

PreparationState::~PreparationState()
{
}

void PreparationState::clear()
{
	delete m_stopwatchText;
	delete m_currentTime;
	delete m_pauseButton;
	delete m_effect;
	delete m_effectTime;
}

void PreparationState::handleInput(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			delete m_pauseButton;

			m_game->changeState(new ToPauseTransition(m_game,
													  m_shared,
													  m_currentTime,
													  m_stopwatchText));
			return;
		}
	}
}

void PreparationState::update(sf::Time elapsed)
{
	m_stateTime += elapsed;

	if (m_stateTime.asSeconds() < PREPARING_DURATION)
	{
		if (m_currentTime > m_shared.bestTime && 
			m_effectTime->asSeconds() < 1.f)
		{
			float a = transfer(*m_effectTime, sf::seconds(1.f), 1.f, 2.f);
			m_sprite.setScale(a, a);
			m_sprite.setColor(transfer(*m_effectTime, sf::seconds(1.f),
									   sf::Color(255, 255, 255, 128),
									   sf::Color(255, 255, 255, 0)));
		}

		float gameSpeedFactor = std::sin(m_currentTime->asSeconds() * PI / 120.f / 2.f) + 1.f;
		float accelerationFactor = std::sin(m_stateTime.asSeconds() / PREPARING_DURATION * PI / 2.f);

		*m_currentTime += elapsed * accelerationFactor;
		m_stopwatchText->setString(getElapsedString(*m_currentTime));

		m_shared.player->update(*m_shared.world, elapsed * accelerationFactor * gameSpeedFactor);

		m_shared.enemySpawner->update(elapsed, accelerationFactor * gameSpeedFactor);

		for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
		{
			(*i)->update(*m_shared.world, elapsed * accelerationFactor * gameSpeedFactor);
			if (m_shared.player->isAlive())
			{
				if (m_shared.player->intersects(*(*i)))
				{
					m_shared.enemySpawner->reset();

					m_game->changeState(new RevivalState(m_game,
														 m_shared,
														 m_currentTime,
														 m_stopwatchText,
														 m_pauseButton));
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
													 m_shared,
													 m_currentTime,
													 m_stopwatchText,
													 m_pauseButton));
			return;
		}
		else
		{
			delete m_pauseButton;

			m_game->changeState(new ToPauseTransition(m_game,
													  m_shared,
													  m_currentTime,
													  m_stopwatchText));
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

	window.setView(window.getDefaultView());
	window.draw(*m_pauseButton);
	window.draw(*m_stopwatchText);
	window.draw(m_sprite);
}

