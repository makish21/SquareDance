#pragma once
#include "GameState.h"
#include "RenderButton.h"

class ToPauseTransition :
	public GameState
{
public:
	ToPauseTransition(Game* const game,
					  const SharedContext& sharedContext);
	~ToPauseTransition();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow& window);

private:
	sf::Time m_elapsedTime;

	sf::RectangleShape* m_blackout;

	RenderButton* m_returnButton;
	RenderButton* m_closeButton;
};

