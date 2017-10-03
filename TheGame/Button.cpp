#include "Button.h"
#include <iostream>


Button::Button()
{
}

Button::Button(sf::Vector2f position, sf::Text text) :
	rect_(sf::Vector2f(text.getGlobalBounds().width + 19.f, text.getCharacterSize() + 19.f), 16.f, 30),
	text_(text)
{
	rect_.setOrigin(rect_.getGlobalBounds().width / 2.f, rect_.getGlobalBounds().height / 2.f);
	rect_.setPosition(position);
	rect_.setFillColor(sf::Color::Transparent);
	rect_.setOutlineColor(sf::Color::Transparent);
	rect_.setOutlineThickness(1.f);

	text_.setOrigin(text_.getGlobalBounds().width / 2.f, text_.getCharacterSize() / 2.f);
	text_.setPosition(position.x - 1, position.y);
}

Button::Button(sf::FloatRect rect) :
	rect_(sf::Vector2f(rect.width, rect.height), 0.f, 2)
{
	rect_.setPosition(rect.left, rect.top);
	rect_.setOutlineColor(sf::Color::Transparent);
}


Button::~Button()
{
}

void Button::setText(sf::Text text)
{
	text_ = text;
	rect_.setSize(sf::Vector2f(text.getGlobalBounds().width + 19.f, text.getCharacterSize() + 19.f));
}

bool Button::isPressed() const
{
	return isPressed_;
}

bool Button::isReleased() const
{
	return isReleased_;
}

void Button::setPosition(sf::Vector2f position)
{
	rect_.setPosition(position);
	text_.setPosition(position);
}

void Button::setSize(sf::Vector2f size)
{
	rect_.setSize(size);
}

void Button::setTextColor(sf::Color color)
{
	text_.setFillColor(color);
}

void Button::setFillColor(sf::Color color)
{
	rect_.setFillColor(color);
}

void Button::setBorderColor(sf::Color color)
{
	rect_.setOutlineColor(color);
}

void Button::setBorderThickness(float thickness)
{
	rect_.setOutlineThickness(thickness);
}

void Button::handleInput(sf::Event& event, sf::Vector2f mouse)
{
#ifdef __ANDROID__
	if (rect_.getGlobalBounds().contains(mouse))
	{
		if (event.type == sf::Event::TouchBegan)
		{
			isPressed_ = true;
		}
		if (event.type == sf::Event::TouchEnded)
		{
			isReleased_ = true;
		}
	}
#else
	if (event.mouseButton.button == sf::Mouse::Left &&
		rect_.getGlobalBounds().contains(mouse))
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			isPressed_ = true;
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			isReleased_ = true;
		}
	}
#endif 
}

void Button::update()
{
	isPressed_ = false;
	isReleased_ = false;
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;

	target.draw(rect_, states);
	target.draw(text_, states);
}
