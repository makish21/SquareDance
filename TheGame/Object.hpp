#pragma once

#include "Collidable.hpp"
#include "World.h"

class Object :
	public sf::Transformable, 
	public sf::Drawable,
	public Collidable
{
public:
	Object() : m_life(true)
	{ }

	virtual ~Object()
	{
	}

	void setLife(bool life)
	{
		m_life = life;
	}

	bool isAlive()
	{
		return m_life;
	}

	virtual void update(World&, sf::Time elapsed) = 0;

protected:
	bool m_life;
};

