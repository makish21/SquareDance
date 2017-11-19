#pragma once
#include <SFML\Graphics.hpp>

#include "Definitions.hpp"
#include "Observer.hpp"

class Background :
	public sf::Transformable,
	public sf::Drawable,
	public Observer
{
public:
	Background(const sf::Vector2u& windowSize) :
		m_drawableArea(static_cast<float>(std::sqrt(std::pow(windowSize.x, 2) +
													std::pow(windowSize.y, 2)))),
		m_color(BACKGROUND_COLOR)
	{
	}
	virtual ~Background(){};

	virtual void setColor(sf::Color color) = 0;

	sf::Color getColor() const
	{
		return m_color;
	}

	virtual void update(sf::Time time) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

protected:
	sf::Color m_color;
	float m_drawableArea;
};