#include "WallEnemy.h"
#include "Definitions.hpp"


WallEnemy::WallEnemy(float top, float bottom, float dirX) :
	m_topEdge(top),
	m_bottomEdge(bottom)
{
	m_dir = dirX;

	m_shape = new sf::RectangleShape(sf::Vector2f(26.f, bottom - top));
	m_shape->setFillColor(sf::Color::White);

	setOrigin(13.f, (bottom - top) / 2.f);
	setArea(std::sqrt(std::pow(13.f, 2) + std::pow((bottom - top) / 2.f, 2)));

	if (dirX > 0)
	{
		setPosition(-50, top + (bottom - top) / 2.f);
	}
	else
	{
		setPosition(850, top + (bottom - top) / 2.f);
	}

	m_velocity = sf::Vector2f(dirX * 0.7f, 0);

	for (int i = 0; i < m_shape->getPointCount(); i++)
	{
		m_collisionVertices[i] = new cd::Vector2<float>();
	}
	setCollision(new cd::ConvexCollision(&m_collisionVertices[0], m_shape->getPointCount()));
	updateCollision();
}


WallEnemy::~WallEnemy()
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

void WallEnemy::update(World & world, sf::Time elapsed)
{
	move(m_velocity * elapsed.asSeconds() * (WORLD_SIZE.y - 20.f));

	checkCollisionWithWorld(world);

	updateCollision();
}

Enemy * WallEnemy::clone()
{
	return new WallEnemy(m_topEdge, m_bottomEdge, m_dir);
}

void WallEnemy::updateCollision()
{
	setCenter(getPosition());
	for (size_t i = 0; i < m_shape->getPointCount(); i++)
	{
		sf::Vector2f vertex = getTransform().transformPoint(m_shape->getPoint(i));
		*m_collisionVertices[i] = cd::Vector2<float>(vertex.x, vertex.y);
	}
}

bool WallEnemy::checkCollisionWithWorld(const World & world)
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
