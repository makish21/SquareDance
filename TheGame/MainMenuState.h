#pragma once
#include "GameState.h"
#include "Button.h"
#include "ToGameTransition.h"

class MainMenuState :
	public GameState
{
public:
	MainMenuState(Game* const game,
				  const SharedContext& sharedContext,
				  sf::Text* const title,
				  sf::Text* const bestTime,
				  sf::Text* const highScore);
	~MainMenuState();

	virtual void clear();

	virtual void draw(sf::RenderWindow& window);
	virtual void update(sf::Time elapsed);
	virtual void handleInput(const sf::Event& event);

private:
	sf::Text* const m_titleText;
	sf::Text* const m_bestTimeText;
	sf::Text* const m_highScoreText;

	Button m_playButton;
};