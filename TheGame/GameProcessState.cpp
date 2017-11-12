#include "GameProcessState.h"
#include "Definitions.hpp"
#include "RevivalState.h"
#include "ToPauseTransition.h"

GameProcessState::GameProcessState(Game* const game,
								   const SharedContext& sharedContext) :
	GameState(game,
			  sharedContext),
	m_stopwatchText("00:00:00", *sharedContext.fileManager->getFont("Helvetica"), 50),
	m_pauseButton(sf::FloatRect(game->getCurrentVideoMode().width - 60,
								0, 60, 60))
{
	m_stopwatchText.setOrigin(0, m_stopwatchText.getLocalBounds().height / 2);
	m_stopwatchText.setPosition(50.f, 100.f);
	m_stopwatchText.setFillColor(sf::Color::White);

	m_pauseButton.setTexture(*sharedContext.fileManager->getTexture("PauseIcon"));
	m_pauseButton.setAlignment(Center, Center);
	m_pauseButton.setFitting(Fit, 0.5f);
}


GameProcessState::~GameProcessState()
{
}

void GameProcessState::handleInput(const sf::Event &event)
{
	sf::Vector2f position;

	if (event.mouseButton.button == sf::Mouse::Left)
	{
		position = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
	}
	if (event.touch.finger == 0)
	{
		position = sf::Vector2f(event.touch.x, event.touch.y);
	}

	if (m_pauseButton.isPressed(event, position))
	{
		m_game->changeState(new ToPauseTransition(m_game,
												  m_shared));
		return;
	}

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

void GameProcessState::update(sf::Time elapsed)
{
	m_sessionTime += elapsed;

	if (m_sessionTime.asSeconds() < 180.f)
	{
		m_speedFactor = std::sin(m_sessionTime.asSeconds() * PI / 120.f / 2.f) + 1.f;
	}

	m_stopwatchText.setString(getElapsedString(m_sessionTime.asMilliseconds()));

	m_shared.player->update(*m_shared.world, elapsed * 1.1874027f/*m_speedFactor*/);

	m_shared.enemySpawner->update(elapsed, m_speedFactor);

	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		(*i)->update(*m_shared.world, elapsed * m_speedFactor);
		if (m_shared.player->isAlive())
		{
			if (m_shared.player->intersects(*(*i)))
			{
				m_game->changeState(new RevivalState(m_game,
													 m_shared));
				return;
			}
		}
	}
}

void GameProcessState::draw(sf::RenderWindow &window)
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
	window.draw(m_stopwatchText);
	window.draw(m_pauseButton);
}

std::string GameProcessState::getElapsedString(sf::Int32 milliseconds)
{
	int ms = milliseconds % 1000 / 10;
	int seconds = (milliseconds / 1000) % 60;
	int minutes = (milliseconds / (1000 * 60)) % 60;
	int hours = (milliseconds / (1000 * 60 * 60)) % 24;

	std::string msString = (ms < 10 ? "0" : "") + std::to_string(ms);
	std::string secString = (seconds < 10 ? "0" : "") + std::to_string(seconds);
	std::string minString = (minutes < 10 ? "0" : "") + std::to_string(minutes);
	std::string hString = (hours < 10 ? "0" : "") + std::to_string(hours);

	return std::string(hours > 0 ? hString + ':' : "" +
					   minString + ':' +
					   secString + ':' +
					   msString);
}
