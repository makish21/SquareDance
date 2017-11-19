#pragma once
#include <SFML\Graphics.hpp>

#include "Background.hpp"

enum Action
{
	Large,
	Medium,

	Count
};

inline Action& operator++(Action& action, int);


class AcceptanceBackground :
	public Background
{
public:
	AcceptanceBackground(const sf::Vector2u& windowSize);
	~AcceptanceBackground();

	virtual void setColor(sf::Color color);

	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	virtual void onNotify(const GameEvent& event);

	bool     m_inAction;
	Action   m_action;
	sf::Time m_actionTime;

	sf::Clock m_clock;

	sf::RectangleShape m_smallRect;
	sf::RectangleShape m_mediumRect;
	sf::RectangleShape m_largeRect;
};
