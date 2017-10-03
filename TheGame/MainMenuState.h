#pragma once
#include "GameState.h"
#include "Button.h"
#include "StatisticsState.h"
#include "ToGameTransition.h"

class MainMenuState :
	public GameState
{
public:
	MainMenuState(Game* game);
	~MainMenuState();

	virtual void draw(sf::RenderWindow& window);
	virtual void update(float time);
	virtual void handleInput(sf::Event& event);

private:
	const float VIEW_ZOOM = 0.5f;
	const sf::Vector2f PLAYER_POSITION = sf::Vector2f(400.f, 240.f);
	const sf::Uint8 WORLD_TRANSPARENCY = 0;
	const sf::Uint8 TITLE_ALPHA = 255;

	sf::Sound m_sound;

	Button m_playButton;
};