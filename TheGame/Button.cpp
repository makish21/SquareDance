#include "Button.h"

Button::Button() :
	m_rect(0.f, 0.f, 0.f, 0.f)
{
}

Button::Button(const sf::FloatRect& rect) :
	m_rect(rect)
{
}


Button::~Button()
{
}

void Button::setRect(const sf::FloatRect & rect)
{
	m_rect = rect;
}

const sf::FloatRect & Button::getRect() const
{
	return m_rect;
}

bool Button::isPressed(const sf::Event & event, const sf::Vector2f & point) const
{
	if (m_rect.contains(point))
	{
		if (event.type == sf::Event::MouseButtonPressed ||
			event.type == sf::Event::TouchBegan)
		{
			return true;
		}
	}

	return false;
}

bool Button::isReleased(const sf::Event & event, const sf::Vector2f & point) const
{
	if (m_rect.contains(point))
	{
		if (event.type == sf::Event::MouseButtonReleased ||
			event.type == sf::Event::TouchEnded)
		{
			return true;
		}
	}
	return false;
}