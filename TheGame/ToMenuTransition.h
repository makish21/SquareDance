#pragma once
#include "Animations.h"
#include "MainMenuState.h"

class ToMenuTransition :
	public GameState
{
public:
	ToMenuTransition(Game* const game,
					 const SharedContext& sharedContext,
					 sf::Time* const currentTime,
					 sf::Text* const stopwatch);

	~ToMenuTransition();

	virtual void clear();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow& window);

private:
	sf::Time m_elapsedTime;

	sf::Text* const m_titleText;
	sf::Text* const m_highScoreText;
	sf::Text* const m_bestTimeText;

	sf::Time* const m_currentTime;
	sf::Text* const m_stopwatchText;

	const sf::Time     c_transitionDuration;

	const float        c_oldViewZoom;
	const sf::Vector2f c_oldPlayerPosition;
	const sf::Vector2f c_oldPlayerScale;
	const sf::Color    c_oldWorldColor;
	const sf::Color    c_oldTitleColor;

	const float        c_newViewZoom;
	const sf::Vector2f c_newPlayerPosition;
	const sf::Vector2f c_newPlayerScale;
	const sf::Color    c_newWorldColor;
	const sf::Color    c_newTitleColor;
};

