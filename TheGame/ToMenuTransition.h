#pragma once
#include "Transition.h"
#include "MainMenuState.h"

class ToMenuTransition :
	public GameState
{
public:
	ToMenuTransition(Game* const game,
					 FileManager* const fileManager,
					 sf::View* const view,
					 Player* const player,
					 EnemySpawner* const enemySpawner,
					 GameObjects* const gameObjects,
					 World* const world);

	~ToMenuTransition();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow& window);

private:
	sf::Time m_elapsedTime;
	const float        c_transitionDuration;

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

