#include "ToMenuTransition.h"
#include "Definitions.hpp"
#include "Functions.hpp"

ToMenuTransition::ToMenuTransition(Game* const game,
								   const SharedContext& sharedContext,
								   sf::Time* const currentTime,
								   sf::Text* const stopwatch) :
	GameState(game,
			  sharedContext),
	m_titleText(new sf::Text),
	m_highScoreText(new sf::Text),
	m_bestTimeText(new sf::Text),
	m_currentTime(currentTime),
	m_stopwatchText(stopwatch),
	c_transitionDuration(sf::seconds(TO_MENU_TRANSITION_DURATION)),
	c_oldPlayerPosition(sharedContext.player->getPosition()),
	c_oldPlayerScale(sharedContext.player->getScale()),
	c_oldViewZoom(game->getViewZoom()),
	c_oldTitleColor(INITIAL_TITLE_COLOR),
	c_oldWorldColor(sharedContext.world->getBoundsColor()),
	c_newViewZoom(MENU_VIEW_ZOOM),
	c_newPlayerPosition(MENU_PLAYER_POSITION),
	c_newPlayerScale(MENU_PLAYER_SCALE),
	c_newWorldColor(MENU_WORLD_COLOR),
	c_newTitleColor(MENU_TITLE_COLOR)
{
	sf::VideoMode bestVM = m_game->getBestVideoMode();
	sf::VideoMode currentVM = m_game->getCurrentVideoMode();

	unsigned int characterSize = CHARACTER_SIZE_FACTOR *
		TITLE_CHARACTER_SIZE *
		bestVM.height;

	m_titleText->setFont(*m_shared.fileManager->getFont("Helvetica"));
	m_titleText->setCharacterSize(characterSize);
	m_titleText->setString(GAME_NAME);
	sf::FloatRect textRect = m_titleText->getLocalBounds();
	m_titleText->setOrigin(textRect.left + textRect.width / 2,
						   textRect.top + textRect.height / 2);
	m_titleText->setScale(float(currentVM.width) / float(bestVM.width),
						  float(currentVM.width) / float(bestVM.width));
	m_titleText->setPosition(static_cast<float>(currentVM.width) / 2,
							 TITLE_Y_POSITION_FACTOR *
							 static_cast<float>(currentVM.height));
	m_titleText->setFillColor(INITIAL_TITLE_COLOR);


	characterSize = CHARACTER_SIZE_FACTOR * 25 * bestVM.height;
	m_bestTimeText->setFont(*m_shared.fileManager->getFont("Helvetica"));
	m_bestTimeText->setCharacterSize(characterSize);
	m_bestTimeText->setString("Best time:");
	m_bestTimeText->setStyle(sf::Text::Style::Bold);
	m_bestTimeText->setFillColor(sf::Color::Transparent);
	textRect = m_bestTimeText->getLocalBounds();
	m_bestTimeText->setOrigin(textRect.left + textRect.width / 2,
							  textRect.top + textRect.height + 5.f);
	m_bestTimeText->setScale(float(currentVM.width) / float(bestVM.width),
							 float(currentVM.width) / float(bestVM.width));
	m_bestTimeText->setPosition(currentVM.width / 2, currentVM.height / 1.3f);

	characterSize = CHARACTER_SIZE_FACTOR * 40 * bestVM.height;
	m_highScoreText->setFont(*m_shared.fileManager->getFont("Helvetica"));
	m_highScoreText->setCharacterSize(characterSize);
	m_highScoreText->setString(getElapsedString(*sharedContext.bestTime));
	m_highScoreText->setFillColor(sf::Color::Transparent);
	textRect = m_highScoreText->getLocalBounds();
	m_highScoreText->setOrigin(textRect.left + textRect.width / 2,
							textRect.top - 5.f);
	m_highScoreText->setScale(float(currentVM.width) / float(bestVM.width),
						   float(currentVM.width) / float(bestVM.width));
	m_highScoreText->setPosition(currentVM.width / 2, currentVM.height / 1.3f);

}


ToMenuTransition::~ToMenuTransition()
{
	// kill all objects
	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		(*i)->setLife(false);
	}

	delete m_currentTime;
	delete m_stopwatchText;
}

void ToMenuTransition::clear()
{
	delete m_titleText;
	delete m_bestTimeText;
	delete m_highScoreText;
}

void ToMenuTransition::handleInput(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed ||
		event.type == sf::Event::MouseButtonPressed ||
		event.type == sf::Event::TouchBegan)
	{
		m_game->changeState(new MainMenuState(m_game,
											  m_shared,
											  m_titleText,
											  m_bestTimeText,
											  m_highScoreText));
		return;
	}
}

void ToMenuTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	m_shared.player->rotate(-10.f * elapsed.asSeconds());

	if (m_elapsedTime <= c_transitionDuration)
	{
		// animation
		m_game->setViewZoom(transfer(m_elapsedTime, c_transitionDuration, c_oldViewZoom, c_newViewZoom));
		m_shared.player->setPosition(transfer(m_elapsedTime, c_transitionDuration, c_oldPlayerPosition, c_newPlayerPosition));
		m_shared.player->setScale(transfer(m_elapsedTime, c_transitionDuration, c_oldPlayerScale, c_newPlayerScale));
		m_titleText->setFillColor(transfer(m_elapsedTime, c_transitionDuration, c_oldTitleColor, c_newTitleColor));
		m_bestTimeText->setFillColor(transfer(m_elapsedTime, c_transitionDuration, sf::Color::Transparent, BEST_TIME_COLOR));
		m_highScoreText->setFillColor(transfer(m_elapsedTime, c_transitionDuration, sf::Color::Transparent, STOPWATCH_COLOR));
		m_shared.world->setBoundsColor(transfer(m_elapsedTime, c_transitionDuration, c_oldWorldColor, c_newWorldColor));

		if (m_stopwatchText)
		{
			m_stopwatchText->setFillColor(transfer(m_elapsedTime,
												   c_transitionDuration,
												   STOPWATCH_COLOR,
												   sf::Color::Transparent));

			// set stopwatch relative to current view
			sf::FloatRect bounds = m_shared.world->getTopBoundRect();
			sf::Vector2f textPosition;
			textPosition.x = bounds.left + bounds.width / 2.f;
			textPosition.y = bounds.top + bounds.height / 2.f;
			textPosition = sf::Vector2f(m_game->mapCoordsToPixel(textPosition,
																 *m_shared.gameView));
			m_stopwatchText->setPosition(textPosition);
		}

		// moving enemies out of screen
		float timeFactor = std::sin(m_elapsedTime.asSeconds() / c_transitionDuration.asSeconds() * PI / 2.f);
		float speedFactor = timeFactor * 10.f + 1.f;

		for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
		{
			(*i)->update(*m_shared.world, elapsed * speedFactor);
		}
	}
	else
	{
		m_game->changeState(new MainMenuState(m_game,
											  m_shared,
											  m_titleText,
											  m_bestTimeText,
											  m_highScoreText));
		return;
	}

}

void ToMenuTransition::draw(sf::RenderWindow & window)
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
	window.draw(*m_bestTimeText);
	window.draw(*m_highScoreText);
	if (m_stopwatchText)
	{
		window.draw(*m_stopwatchText);
	}
	window.draw(*m_titleText);
}

