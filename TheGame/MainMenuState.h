#pragma once
#include "GameState.h"
#include "Button.h"
#include "ToGameTransition.h"

class MainMenuState :
	public GameState
{
public:
	MainMenuState(Game* const game,
				  const SharedContext& sharedContext);
	~MainMenuState();

	virtual void draw(sf::RenderWindow& window);
	virtual void update(sf::Time elapsed);
	virtual void handleInput(const sf::Event& event);

private:
	Button m_playButton;
};