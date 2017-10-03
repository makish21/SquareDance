#include "ContentsBar.h"



ContentsBar::ContentsBar(const sf::String & left, const sf::String & right, sf::Font & font, unsigned int characterSize)
	: left_(left, font, characterSize),
	right_(right, font, characterSize)
{
	left_.setOrigin(left_.getGlobalBounds().left, left_.getGlobalBounds().height / 2);
	right_.setOrigin(right_.getGlobalBounds().width, right_.getGlobalBounds().height / 2);
}

ContentsBar::~ContentsBar()
{
}

void ContentsBar::setColor(sf::Color& color)
{
	left_.setFillColor(color);
	right_.setFillColor(color);
}

void ContentsBar::setPosition(sf::Vector2f position, float width)
{
	left_.setPosition(position);
	right_.setPosition(position.x + width, position.y);
}

void ContentsBar::draw(sf::RenderWindow & window) const
{
	window.draw(left_);
	window.draw(right_);
}
