#pragma once
#include "Animations.h"
#include "GameProcessState.h"

class ToGameTransition :
	public GameState
{
public:
	ToGameTransition(Game* const game,
					 const SharedContext& sharedContext,
					 sf::Text* const title,
					 sf::Text* const bestTime,
					 sf::Text* const highScore);
	~ToGameTransition();

	virtual void clear();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow& window);

private:
	sf::Time m_elapsedTime;

	sf::Text* const m_titleText;
	sf::Text* const m_bestTimeText;
	sf::Text* const m_highScoreText;

	sf::Text* const m_stopwatchText;

	const sf::Time c_transitionDuration;

	const float c_oldPlayerRotation;
	const float c_newPlayerRotation;

	const float        c_oldViewZoom;
	const sf::Vector2f c_oldPlayerPosition;
	const sf::Color    c_oldWorldColor;
	const sf::Color    c_oldTitleColor;

	const float        c_newViewZoom;
	const sf::Vector2f c_newPlayerPosition;
	const sf::Color    c_newWorldColor;
	const sf::Color    c_newTitleColor;
};

