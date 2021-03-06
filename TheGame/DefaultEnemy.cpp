#include "DefaultEnemy.h"
#include "Definitions.hpp"

#define RADIUS 14.14213f

DefaultEnemy::DefaultEnemy(float y, float dirX)
{
	m_dir = dirX;
	m_velocity.y = 0;
	m_shape = new sf::CircleShape(RADIUS, 5);
	m_shape->setFillColor(sf::Color(110, 255, 231));

	setOrigin(RADIUS, RADIUS);
	setArea(RADIUS);

	if (dirX > 0)
	{
		setPosition(-50, y);
	}
	else
	{
		setPosition(850, y);
	}

	m_velocity.x = dirX * 0.7f;

	for (int i = 0; i < m_shape->getPointCount(); i++)
	{
		m_collisionVertices[i] = new cd::Vector2<float>();
	}

	setCollision(new cd::ConvexCollision(&m_collisionVertices[0], 5));

	updateCollision();
}

void DefaultEnemy::update(World& world, sf::Time elapsed)
{
	move(m_velocity * elapsed.asSeconds() * (WORLD_SIZE.y - 20.f));
	rotate(m_velocity.x * elapsed.asSeconds() * (WORLD_SIZE.y - 20.f));

	checkCollisionWithWorld(world);

	updateCollision();
}

Enemy * DefaultEnemy::clone()
{
	return new DefaultEnemy(getPosition().y, m_dir);
}

void DefaultEnemy::updateCollision()
{
	setCenter(getPosition());
	for (size_t i = 0; i < m_shape->getPointCount(); i++)
	{
		sf::Vector2f vertex = getTransform().transformPoint(m_shape->getPoint(i));
		*m_collisionVertices[i] = cd::Vector2<float>(vertex.x, vertex.y);
	}
}

bool DefaultEnemy::checkCollisionWithWorld(const World& world)
{
	if (getPosition().x > world.getBounds().left + world.getBounds().width + 50)
	{
		m_life = false;
		return true;
	}
	else if (getPosition().x < world.getBounds().left - 50)
	{
		m_life = false;
		return true;
	}
	else
	{
		return false;
	}
}


DefaultEnemy::~DefaultEnemy()
{
	for (int i = 0; i < m_shape->getPointCount(); i++)
	{
		if (m_collisionVertices[i])
		{
			delete m_collisionVertices[i];
			m_collisionVertices[i] = nullptr;
		}
	}
}
