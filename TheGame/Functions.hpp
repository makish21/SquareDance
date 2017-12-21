#pragma once

#include <SFML\Graphics.hpp>

// Functions
static std::string getElapsedString(sf::Time time)
{
	sf::Int32 milliseconds = time.asMilliseconds();

	int ms = milliseconds % 1000 / 10;
	int seconds = (milliseconds / 1000)/* % 60*/;

	std::string msString = (ms < 10 ? "0" : "") + std::to_string(ms);
	std::string secString = /*(seconds < 10 ? "0" : "") +*/ std::to_string(seconds);

	return std::string(secString + '.' +
					   msString);
}