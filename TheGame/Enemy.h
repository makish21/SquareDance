#pragma once
#include <SFML\Graphics.hpp>

#include "Object.hpp"
#include "World.h"

class Enemy :
	public Object
{
public:
	Enemy();
	Enemy(float x, float y);
	virtual ~Enemy();

	virtual void update(World&, sf::Time elapsed) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual Enemy* clone() = 0;

protected:
	float m_dir;
	sf::Vector2f m_velocity;
	sf::Shape* m_shape;
};
