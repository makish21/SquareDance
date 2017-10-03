#pragma once
#include "GameState.h"
#include "MainMenuState.h"
#include "Button.h"
#include "ContentsBar.h"

class StatisticsState :
	public GameState
{
public:
	StatisticsState(Game* game);
	~StatisticsState();

	virtual void draw(sf::RenderWindow& window);
	virtual void update(float time);
	virtual void handleInput(sf::Event& event);

private:
	ContentsBar total_;
	ContentsBar score_;
	ContentsBar bestScore_;
	ContentsBar deaths_;

	sf::Text back_;
	Button return_;
};

