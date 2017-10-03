#pragma once
#include <SFML\Graphics.hpp>
class ContentsBar
{
public:
	ContentsBar(const sf::String& left, const sf::String& right, sf::Font& font, unsigned int characterSize);
	~ContentsBar();

	void setColor(sf::Color& color);
	void setPosition(sf::Vector2f position, float width);

	void draw(sf::RenderWindow& window) const;

private:
	sf::Text left_;
	sf::Text right_;
};

