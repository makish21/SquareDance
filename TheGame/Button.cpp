#include "Button.h"

Button::Button(const sf::Vector2f & position, const sf::Vector2f & size) :
	m_rect(position.x, position.y,
		   size.x, size.y)
{
}

Button::Button(sf::FloatRect rect) :
	m_rect(rect)
{

}


Button::~Button()
{
}

void Button::setPosition(sf::Vector2f position)
{
	m_rect.left = position.x;
	m_rect.top = position.y;
}

void Button::setSize(sf::Vector2f size)
{
	m_rect.width = size.x;
	m_rect.height = size.y;
}

bool Button::isPressed(const sf::Event & event, const sf::Vector2f & mouse) const
{
	if ((event.mouseButton.button == sf::Mouse::Left ||
		 event.touch.finger == 0) &&
		m_rect.contains(mouse))
	{
		if (event.type == sf::Event::MouseButtonPressed ||
			event.type == sf::Event::TouchBegan)
		{
			return true;
		}
	}

	return false;
}

bool Button::isReleased(const sf::Event & event, const sf::Vector2f & mouse) const
{
	if ((event.mouseButton.button == sf::Mouse::Left ||
		 event.touch.finger == 0) &&
		m_rect.contains(mouse))
	{
		if (event.type == sf::Event::MouseButtonReleased ||
			event.type == sf::Event::TouchEnded)
		{
			return true;
		}
	}
	return false;
}