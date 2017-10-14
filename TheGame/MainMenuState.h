#pragma once
#include "GameState.h"
#include "Button.h"
#include "ToGameTransition.h"

class MainMenuState :
	public GameState
{
public:
	MainMenuState(Game* game);
	~MainMenuState();

	virtual void draw(sf::RenderWindow& window);
	virtual void update(sf::Time elapsed);
	virtual void handleInput(sf::Event& event);

private:
	const sf::Uint8 WORLD_TRANSPARENCY = 0;

	sf::Sound m_sound;

	Button m_playButton;
};