#include "Animations.h"

float transfer(sf::Time elapsed, sf::Time duration, float oldVal, float newVal)
{
	float factor = 1.f - std::sin(elapsed.asSeconds() / duration.asSeconds() * PI / 2.f);

	return (oldVal - newVal) * factor + newVal;
}

sf::Uint8 transfer(sf::Time elapsed, sf::Time duration, sf::Uint8 oldVal, sf::Uint8 newVal)
{
	float factor = 1.f - std::sin(elapsed.asSeconds() / duration.asSeconds() * PI / 2.f);

	return static_cast<sf::Uint8>((oldVal - newVal) * factor + newVal);
}

sf::Vector2f transfer(sf::Time elapsed, sf::Time duration, sf::Vector2f oldVal, sf::Vector2f newVal)
{
	float factor = 1.f - std::sin(elapsed.asSeconds() / duration.asSeconds() * PI / 2.f);

	return (oldVal - newVal) * factor + newVal;
}

sf::Color transfer(sf::Time elapsed, sf::Time duration, sf::Color oldVal, sf::Color newVal)
{
	float factor = 1.f - std::sin(elapsed.asSeconds() / duration.asSeconds() * PI / 2.f);

	sf::Color color;

	color.r = static_cast<sf::Uint8>((oldVal.r - newVal.r) * factor + newVal.r);
	color.g = static_cast<sf::Uint8>((oldVal.g - newVal.g) * factor + newVal.g);
	color.b = static_cast<sf::Uint8>((oldVal.b - newVal.b) * factor + newVal.b);
	color.a = static_cast<sf::Uint8>((oldVal.a - newVal.a) * factor + newVal.a);

	return color;
}
