#pragma once
#include <SFML\Graphics.hpp>
#include "RoundedRectangleShape.h"
class Button
{
public:
	Button(const sf::Vector2f& position = sf::Vector2f(0.f, 0.f),
		   const sf::Vector2f& size = sf::Vector2f(0.f, 0.f));
	Button(sf::FloatRect rect);
	~Button();

	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);

	bool isPressed(const sf::Event& event, const sf::Vector2f& mouse) const;
	bool isReleased(const sf::Event& event, const sf::Vector2f& mouse) const;

private:
	sf::FloatRect m_rect;
};

