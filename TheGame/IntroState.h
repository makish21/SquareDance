#pragma once
#include <SFML\Graphics.hpp>
#include <thread>

#include "GameState.h"

class IntroState :
	public GameState
{
public:
	IntroState(Game* const game,
			   const SharedContext& sharedContext);
	~IntroState();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow& window);

private:
	void loadData();
	std::thread m_loadDataThread;

	sf::Time m_elapsedTime;

	sf::Color m_textColor;

	sf::Text m_developerText;
	sf::Text m_developerNameText;
	sf::Text m_createdWithText;
};

