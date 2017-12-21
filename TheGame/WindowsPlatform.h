#pragma once
#include <SFML\System.hpp>

#ifdef SFML_SYSTEM_WINDOWS
#include <fstream>
#include "Platform.h"

class WindowsPlatform :
	public Platform
{
public:
	WindowsPlatform();

	~WindowsPlatform();

private:

	virtual int vibrate(sf::Time duration);
	virtual int saveHighScore(sf::Time score);
	virtual int loadHighScore(sf::Time& time);
};

#endif // SFML_SYSTEM_WINDOWS