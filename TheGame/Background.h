#pragma once
#include <SFML\Graphics.hpp>

class Background :
	public sf::Transformable,
	public sf::Drawable
{
public:
	Background(const sf::Vector2u& windowSize);
	~Background();

	void update(sf::Time elapsed);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	float m_drawableArea;

	sf::Clock m_clock;

	sf::Color m_color;

	sf::RectangleShape m_smallRect;
	sf::RectangleShape m_mediumRect;
	sf::RectangleShape m_largeRect;
};

