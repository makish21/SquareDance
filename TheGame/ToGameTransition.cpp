#include "ToGameTransition.h"
#include "Definitions.hpp"
#include "PreparationState.h"

ToGameTransition::ToGameTransition(Game* const game,
								   const SharedContext& sharedContext,
								   sf::Text* const title,
								   sf::Text* const bestTime,
								   sf::Text* const highScore) :
	GameState(game,
			  sharedContext),
	m_titleText(title),
	m_bestTimeText(bestTime),
	m_highScoreText(highScore),
	m_stopwatchText(new sf::Text),
	c_transitionDuration(sf::seconds(TO_GAME_TRANSITION_DURATION)),
	c_oldPlayerPosition(sharedContext.player->getPosition()),
	c_oldViewZoom(game->getViewZoom()),
	c_oldTitleColor(title->getFillColor()),
	c_oldWorldColor(sharedContext.world->getBoundsColor()),
	c_newViewZoom(GAME_VIEW_ZOOM),
	c_newPlayerPosition(GAME_PLAYER_POSITION),
	c_newWorldColor(GAME_WORLD_COLOR),
	c_newTitleColor(GAME_TITLE_COLOR),
	c_oldPlayerRotation(sharedContext.player->getRotation()),
	c_newPlayerRotation(c_oldPlayerRotation - (std::fmod(c_oldPlayerRotation, 90.f)) + 45.f)
{
	sf::VideoMode bestVM = m_game->getBestVideoMode();
	sf::VideoMode currentVM = m_game->getCurrentVideoMode();

	unsigned int characterSize = CHARACTER_SIZE_FACTOR * 40 * bestVM.height;

	sf::FloatRect bounds = sharedContext.world->getTopBoundRect();

	sf::Vector2f textPosition;
	textPosition.x = bounds.left + bounds.width / 2.f;
	textPosition.y = bounds.top + bounds.height / 2.f;

	textPosition = sf::Vector2f(game->mapCoordsToPixel(textPosition,
													   *sharedContext.gameView));

	m_stopwatchText->setFont(*m_shared.fileManager->getFont("Titles"));
	m_stopwatchText->setCharacterSize(characterSize);
	m_stopwatchText->setString("0.00");
	sf::FloatRect textRect = m_stopwatchText->getLocalBounds();
	m_stopwatchText->setOrigin(textRect.left + textRect.width / 2,
							   textRect.top + textRect.height / 2);
	m_stopwatchText->setPosition(textPosition);
	m_stopwatchText->setScale(float(currentVM.width) / float(bestVM.width),
							  float(currentVM.width) / float(bestVM.width));
	m_stopwatchText->setFillColor(sf::Color(255, 255, 255, 0));
}

ToGameTransition::~ToGameTransition()
{
	delete m_titleText;
	delete m_bestTimeText;
	delete m_highScoreText;
}

void ToGameTransition::clear()
{
	delete m_stopwatchText;
}

void ToGameTransition::handleInput(const sf::Event & event)
{
}

void ToGameTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	if (m_elapsedTime <= c_transitionDuration)
	{
		m_game->setViewZoom(transfer(m_elapsedTime, c_transitionDuration, c_oldViewZoom, c_newViewZoom));
		m_shared.player->setPosition(transfer(m_elapsedTime, c_transitionDuration, c_oldPlayerPosition, c_newPlayerPosition));
		m_titleText->setFillColor(transfer(m_elapsedTime, c_transitionDuration, c_oldTitleColor, c_newTitleColor));
		m_bestTimeText->setFillColor(transfer(m_elapsedTime, c_transitionDuration, BEST_TIME_COLOR, sf::Color::Transparent));
		m_highScoreText->setFillColor(transfer(m_elapsedTime, c_transitionDuration, STOPWATCH_COLOR, sf::Color::Transparent));
		m_stopwatchText->setFillColor(transfer(m_elapsedTime, c_transitionDuration, sf::Color::Transparent, STOPWATCH_COLOR));
		m_shared.world->setBoundsColor(transfer(m_elapsedTime, c_transitionDuration, c_oldWorldColor, c_newWorldColor));

		sf::FloatRect bounds = m_shared.world->getTopBoundRect();
		sf::Vector2f textPosition;
		textPosition.x = bounds.left + bounds.width / 2.f;
		textPosition.y = bounds.top + bounds.height / 2.f;
		textPosition = sf::Vector2f(m_game->mapCoordsToPixel(textPosition,
															 *m_shared.gameView));
		m_stopwatchText->setPosition(textPosition);

		m_shared.player->setRotation(transfer(m_elapsedTime,
											  c_transitionDuration,
											  c_oldPlayerRotation,
											  c_newPlayerRotation));
	}
	else
	{
		m_game->changeState(new PreparationState(m_game,
												 m_shared,
												 new sf::Time,
												 m_stopwatchText));
		return;
	}
}

void ToGameTransition::draw(sf::RenderWindow & window)
{
	window.draw(*m_shared.background);

	window.setView(*m_shared.gameView);

	window.draw(*m_shared.world);
	window.draw(*m_shared.player);

	window.setView(window.getDefaultView());
	window.draw(*m_bestTimeText);
	window.draw(*m_highScoreText);
	window.draw(*m_titleText);
	window.draw(*m_stopwatchText);
}


