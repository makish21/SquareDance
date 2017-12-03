#pragma once
#include "GameState.h"

float transfer(sf::Time elapsed, sf::Time duration, float oldVal, float newVal);
sf::Uint8 transfer(sf::Time elapsed, sf::Time duration, sf::Uint8 oldVal, sf::Uint8 newVal);
sf::Vector2f transfer(sf::Time elapsed, sf::Time duration, sf::Vector2f oldVal, sf::Vector2f newVal);
sf::Color transfer(sf::Time elapsed, sf::Time duration, sf::Color oldVal, sf::Color newVal);

float pulse(sf::Time time, sf::Time duration, float average, float spreading);