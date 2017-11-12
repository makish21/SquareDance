#pragma once
#include "GameState.h"
#include "RenderButton.h"

class PauseState :
	public GameState
{
public:
	PauseState(Game* const game,
			   const SharedContext& sharedContext,
			   sf::RectangleShape* blackout,
			   RenderButton* returnButton,
			   RenderButton* closeButton);
	~PauseState();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow& window);

private:
	sf::RectangleShape* m_blackout;

	RenderButton* m_returnButton;
	RenderButton* m_closeButton;
};

