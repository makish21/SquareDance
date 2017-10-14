#include "DefaultEnemy.h"

#define RADIUS 14.14213f

DefaultEnemy::DefaultEnemy(float y, float velocityX)
{
	m_velocity.y = 0;

	m_shape.setRadius(RADIUS);

	setOrigin(RADIUS, RADIUS);
	setArea(RADIUS);

	m_shape.setFillColor(sf::Color(110,255,231));
	m_shape.setPointCount(5);

	if (velocityX > 0)
	{
		setPosition(-50, y);
	}
	else
	{
		setPosition(850, y);
	}

	m_velocity.x = velocityX;

	for (int i = 0; i < 5; i++)
	{
		m_collisionVertices[i] = new cd::Vector2<float>();
	}

	setCollision(new cd::ConvexCollision(&m_collisionVertices[0], 5));

	updateCollision();
}

void DefaultEnemy::update(World& world, sf::Time elapsed)
{
	checkCollisionWithWorld(world);

	move(m_velocity * static_cast<float>(elapsed.asMilliseconds()) / 11.5f);
	rotate(m_velocity.x * static_cast<float>(elapsed.asMilliseconds()) / 11.5f);

	updateCollision();
}

Enemy * DefaultEnemy::clone()
{
	return new DefaultEnemy(getPosition().y, m_velocity.x);
}

void DefaultEnemy::updateCollision()
{
	setCenter(getPosition());
	for (size_t i = 0; i < 5; i++)
	{
		sf::Vector2f vertex = getTransform().transformPoint(m_shape.getPoint(i));
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
	for (int i = 0; i < 5; i++)
	{
		if (m_collisionVertices[i])
		{
			delete m_collisionVertices[i];
			m_collisionVertices[i] = nullptr;
		}
	}
}
