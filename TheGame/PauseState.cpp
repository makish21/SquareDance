#include "PauseState.h"
#include "PreparationState.h"
#include "MainMenuState.h"

PauseState::PauseState(Game * const game,
					   const SharedContext& sharedContext,
					   sf::RectangleShape* blackout,
					   RenderButton* returnButton,
					   RenderButton* closeButton) :
	GameState(game,
			  sharedContext),
	m_blackout(blackout),
	m_returnButton(returnButton),
	m_closeButton(closeButton)
{
	m_blackout->setFillColor(sf::Color(0, 0, 0, 128));

	m_returnButton->setRect(sf::FloatRect(game->getCurrentVideoMode().width - 250,
										  game->getCurrentVideoMode().height - 150,
										  100, 100));

	m_closeButton->setRect(sf::FloatRect(150, game->getCurrentVideoMode().height - 150,
										 100, 100));
}

PauseState::~PauseState()
{
	delete m_blackout;
	delete m_returnButton;
	delete m_closeButton;
}

void PauseState::handleInput(const sf::Event & event)
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

	if (m_returnButton->isReleased(event, position))
	{
		m_game->changeState(new PreparationState(m_game,
												 m_shared));
		return;
	}
	if (m_closeButton->isReleased(event, position))
	{
		m_game->changeState(new ToMenuTransition(m_game,
												 m_shared));
		return;
	}
}

void PauseState::update(sf::Time elapsed)
{
}

void PauseState::draw(sf::RenderWindow & window)
{
	window.draw(*m_shared.background);

	window.setView(*m_shared.gameView);

	window.draw(*m_shared.world);

	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		window.draw(**i);
	}

	window.setView(window.getDefaultView());
	window.draw(*m_blackout);

	window.setView(*m_shared.gameView);
	window.draw(*m_shared.player);

	window.setView(window.getDefaultView());
	window.draw(*m_returnButton);
	window.draw(*m_closeButton);
}
