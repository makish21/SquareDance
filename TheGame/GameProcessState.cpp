#include "GameProcessState.h"
#include "Definitions.hpp"
#include "Functions.hpp"
#include "RevivalState.h"
#include "ToPauseTransition.h"

GameProcessState::GameProcessState(Game* const game,
								   const SharedContext& sharedContext,
								   sf::Time* const currentTime,
								   sf::Text* const stopwatch,
								   RenderButton* const pauseButton) :
	GameState(game,
			  sharedContext),
	m_stopwatchText(stopwatch),
	m_currentTime(currentTime),
	m_pauseButton(pauseButton)
{
	sf::Text text("0.00",
				  *m_shared.fileManager->getFont("Titles"),
				  m_stopwatchText->getCharacterSize());
	sf::FloatRect rect = text.getLocalBounds();
	text.setString(getElapsedString(*m_shared.bestTime));
}


GameProcessState::~GameProcessState()
{
}

void GameProcessState::clear()
{
	delete m_currentTime;
	delete m_stopwatchText;
	delete m_pauseButton;
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

	if (m_pauseButton->isPressed(event, position))
	{
		delete m_pauseButton;
		m_game->changeState(new ToPauseTransition(m_game,
												  m_shared,
												  m_currentTime,
												  m_stopwatchText));
		return;
	}

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
	if (event.type == sf::Event::LostFocus)
	{
		delete m_pauseButton;
		m_game->changeState(new ToPauseTransition(m_game,
												  m_shared,
												  m_currentTime,
												  m_stopwatchText));
		return;
	}
}

void GameProcessState::update(sf::Time elapsed)
{
	*m_currentTime += elapsed;

	/*if (m_currentTime->asSeconds() < 180.f)
	{
		m_speedFactor = std::sin(m_currentTime->asSeconds() * PI / 120.f / 2.f) + 1.f;
	}*/

	m_speedFactor = 1.3f;

	m_stopwatchText->setString(getElapsedString(*m_currentTime));

	m_shared.player->update(*m_shared.world, elapsed * /*1.1874027f*/m_speedFactor);

	m_shared.enemySpawner->update(elapsed, m_speedFactor);

	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		(*i)->update(*m_shared.world, elapsed * m_speedFactor);
		if (m_shared.player->isAlive())
		{
			if (m_shared.player->intersects(*(*i)))
			{
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
	window.draw(*m_stopwatchText);
	//window.draw(m_sprite);
	window.draw(*m_pauseButton);
}
