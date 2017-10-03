#pragma once
#include <SFML\Graphics.hpp>
#include "RoundedRectangleShape.h"
class Button : 
	public sf::Drawable, sf::Transformable
{
public:
	Button();
	Button(sf::Vector2f position, sf::Text text);
	Button(sf::FloatRect rect);
	~Button();

	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);

	void setTextColor(sf::Color);
	void setFillColor(sf::Color);
	void setBorderColor(sf::Color);
	void setBorderThickness(float thickness);

	void setText(sf::Text text);

	bool isPressed() const;
	bool isReleased() const;

	void handleInput(sf::Event& event, sf::Vector2f mouse);
	void update();
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
	bool isPressed_, isReleased_;
	sf::RoundedRectangleShape rect_;
	sf::Text text_;
};

