#pragma once
#include <SFML\System.hpp>

class Platform
{
public:
	static Platform& instance();

	virtual ~Platform();

	virtual int vibrate(sf::Time duration) = 0;
	virtual int saveHighScore(sf::Time score) = 0;
	virtual int loadHighScore(sf::Time& time) = 0;

protected:
	Platform();
};

