#include "ToPauseTransition.h"
#include "PauseState.h"
#include "Animations.h"

ToPauseTransition::ToPauseTransition(Game* const game,
									 const SharedContext& sharedContext) :
	GameState(game,
			  sharedContext),
	m_blackout(new sf::RectangleShape(sf::Vector2f(game->getCurrentVideoMode().width,
												   game->getCurrentVideoMode().height))),
	m_returnButton(new RenderButton(sf::FloatRect(game->getCurrentVideoMode().width - 250,
												  game->getCurrentVideoMode().height,
												  100, 100))),
	m_closeButton(new RenderButton(sf::FloatRect(150, game->getCurrentVideoMode().height,
												 100, 100)))
{
	m_blackout->setFillColor(sf::Color(0, 0, 0, 0));

	m_returnButton->setTexture(*sharedContext.fileManager->getTexture("ResumeIcon"));
	m_returnButton->setAlignment(Center, Center);
	m_returnButton->setFitting(Fit, 0.5f);

	m_closeButton->setTexture(*sharedContext.fileManager->getTexture("ExitIcon"));
	m_closeButton->setAlignment(Center, Center);
	m_closeButton->setFitting(Fit, 0.4f);
}


ToPauseTransition::~ToPauseTransition()
{
}

void ToPauseTransition::handleInput(const sf::Event & event)
{
}

void ToPauseTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	float buttonsTop;
	sf::FloatRect rect;

	if (m_elapsedTime.asSeconds() < 0.3f)
	{
		// Change blackout Alpha
		m_blackout->setFillColor(transfer(m_elapsedTime, sf::seconds(0.3f),
										   sf::Color(0, 0, 0, 0),
										   sf::Color(0, 0, 0, 128)));
		// Change Y button coordinate
		buttonsTop = transfer(m_elapsedTime, sf::seconds(0.3f),
							   m_game->getCurrentVideoMode().height,
							   m_game->getCurrentVideoMode().height - 150.f);

		// Change return button position
		rect = m_returnButton->getRect();
		rect.top = buttonsTop;
		m_returnButton->setRect(rect);

		// Change close button position
		rect = m_closeButton->getRect();
		rect.top = buttonsTop;
		m_closeButton->setRect(rect);
	}
	else
	{
		m_game->changeState(new PauseState(m_game,
										   m_shared,
										   m_blackout,
										   m_returnButton,
										   m_closeButton));

		return;
	}
}

void ToPauseTransition::draw(sf::RenderWindow & window)
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
