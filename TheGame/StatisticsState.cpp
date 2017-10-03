#include "StatisticsState.h"

StatisticsState::StatisticsState(Game * game) :
	GameState(game),
	total_("Total Time", "00:02:48", *game->getFont("Futurica"), 30),
	score_("Score", "00:01:56", *game->getFont("Futurica"), 30),
	bestScore_("Best Score", "00:01:56", *game->getFont("Futurica"), 30),
	deaths_("Deaths", "4583", *game->getFont("Futurica"), 30),
	back_(")", *game->getFont("Buttons"), 42),
	return_(sf::Vector2f(732, 412), back_)
{
	sf::Color color(255, 215, 215);

	total_.setPosition(sf::Vector2f(154, 96), 492);
	total_.setColor(color);

	score_.setPosition(sf::Vector2f(154, 192), 492);
	score_.setColor(color);

	bestScore_.setPosition(sf::Vector2f(154, 288), 492);
	bestScore_.setColor(color);

	deaths_.setPosition(sf::Vector2f(154, 384), 492);
	deaths_.setColor(color);

	back_.setFillColor(sf::Color(186, 134, 134));
	return_.setTextColor(sf::Color(186, 134, 134));
}

StatisticsState::~StatisticsState()
{
}

void StatisticsState::draw(sf::RenderWindow & window)
{
	window.draw(*m_game->getWorld());

	total_.draw(window);
	score_.draw(window);
	bestScore_.draw(window);
	deaths_.draw(window);

	window.draw(return_);
}

void StatisticsState::update(float time)
{
	if (return_.isPressed())
	{
		m_game->popState();
	}

	return_.update();
}

void StatisticsState::handleInput(sf::Event& event)
{
	while (m_game->getWindow()->pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_game->changeState(new MainMenuState(m_game));
				return;
			}
		}
	}

	return_.handleInput(event, m_game->getWorldMousePosition());
}
