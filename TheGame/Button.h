#pragma once
#include <SFML\Graphics.hpp>
#include "RoundedRectangleShape.h"
class Button
{
public:
	Button();
	Button(const sf::FloatRect& rect);
	~Button();

	virtual void setRect(const sf::FloatRect& rect);
	const sf::FloatRect& getRect() const;

	bool isPressed(const sf::Event& event, const sf::Vector2f& point) const;
	bool isReleased(const sf::Event& event, const sf::Vector2f& point) const;

protected:
	sf::FloatRect m_rect;
};

