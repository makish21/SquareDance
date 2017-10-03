#include "Collidable.hpp"

Collidable::Collidable() :
	m_collision(nullptr)
{
}

Collidable::Collidable(cd::Collision * collision) :
	m_collision(collision)
{
}

Collidable::~Collidable()
{
}

bool Collidable::intersects(const Collidable & other)
{
	sf::Vector2f distance = m_position - other.m_position;
	
	float length = distance.x * distance.x + distance.y * distance.y;

	if (length <= (m_area + other.m_area) * (m_area + other.m_area))
	{
		if (m_collision->intersects(*other.m_collision))
		{
			return true;
		}
	}

	return false;
}

void Collidable::setCollision(cd::Collision * collision)
{
	m_collision = collision;
}

cd::Collision * Collidable::getCollision() const
{
	return m_collision;
}

void Collidable::setCenter(const sf::Vector2f & position)
{
	m_position = position;
}

void Collidable::setArea(float radius)
{
	m_area = radius;
}
