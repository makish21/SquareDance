#include "GameProcessState.h"
#include "Definitions.hpp"
#include "Functions.hpp"
#include "RevivalState.h"
#include "ToPauseTransition.h"

GameProcessState::GameProcessState(Game* const game,
								   const SharedContext& sharedContext,
								   sf::Time* const currentTime,
								   sf::Text* const stopwatch) :
	GameState(game,
			  sharedContext),
	m_stopwatchText(stopwatch),
	m_currentTime(currentTime),
	m_pauseButton(sf::FloatRect(game->getCurrentVideoMode().width - 60,
								0, 60, 60)),
	m_effect(new sf::RenderTexture),
	m_effectTime(new sf::Time(sf::Time::Zero))
{
	m_pauseButton.setTexture(*sharedContext.fileManager->getTexture("PauseIcon"));
	m_pauseButton.setAlignment(Center, Center);
	m_pauseButton.setFitting(Fit, 0.5f);

	sf::Text text("00:00.00",
				  *m_shared.fileManager->getFont("Helvetica"),
				  m_stopwatchText->getCharacterSize());
	sf::FloatRect rect = text.getLocalBounds();
	m_effect->create(rect.left + rect.width,
					 rect.top + rect.height);
	text.setString(getElapsedString(*m_shared.bestTime));
	m_effect->clear(sf::Color::Transparent);
	m_effect->draw(text);
	m_effect->display();

	m_sprite.setTexture(m_effect->getTexture());
	rect = m_sprite.getLocalBounds();
	m_sprite.setOrigin(rect.left + rect.width / 2,
					   rect.top /*+ rect.height / 2*/);
	m_sprite.setPosition(m_stopwatchText->getPosition().x,
						 m_stopwatchText->getGlobalBounds().top);
}


GameProcessState::~GameProcessState()
{
	delete m_effect;
	delete m_effectTime;
}

void GameProcessState::clear()
{
	delete m_currentTime;
	delete m_stopwatchText;
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
												  m_shared,
												  m_currentTime,
												  m_stopwatchText));
		return;
	}

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			m_game->changeState(new ToPauseTransition(m_game,
													  m_shared,
													  m_currentTime,
													  m_stopwatchText));
			return;
		}
	}
	if (event.type == sf::Event::LostFocus)
	{
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

	if (m_currentTime->asSeconds() < 180.f)
	{
		m_speedFactor = std::sin(m_currentTime->asSeconds() * PI / 120.f / 2.f) + 1.f;
	}

	if (m_currentTime > m_shared.bestTime &&
		m_effectTime->asSeconds() < 1.f)
	{
		float a = transfer(*m_effectTime, sf::seconds(1.f), 1.f, 2.f);
		m_sprite.setScale(a, a);
		m_sprite.setColor(transfer(*m_effectTime, sf::seconds(1.f),
								   sf::Color(255, 255, 255, 128),
								   sf::Color(255, 255, 255, 0)));
	}

	m_stopwatchText->setString(getElapsedString(*m_currentTime));

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
													 m_shared,
													 m_currentTime,
													 m_stopwatchText));
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
	window.draw(m_pauseButton);
}
