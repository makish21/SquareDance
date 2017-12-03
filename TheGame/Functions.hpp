#pragma once

#include <SFML\Graphics.hpp>

// Functions
static std::string getElapsedString(sf::Time time)
{
	sf::Int32 milliseconds = time.asMilliseconds();

	int ms = milliseconds % 1000 / 10;
	int seconds = (milliseconds / 1000) % 60;
	int minutes = (milliseconds / (1000 * 60)) % 60;
	int hours = (milliseconds / (1000 * 60 * 60)) % 24;

	std::string msString = (ms < 10 ? "0" : "") + std::to_string(ms);
	std::string secString = (seconds < 10 ? "0" : "") + std::to_string(seconds);
	std::string minString = (minutes < 10 ? "0" : "") + std::to_string(minutes);
	std::string hString = (hours < 10 ? "0" : "") + std::to_string(hours);

	return std::string((hours > 0 ? hString + ':' : "") +
					   minString + ':' +
					   /*(minutes > 0 ? minString + ':' : "") +*/
					   secString + '.' +
					   msString);
}