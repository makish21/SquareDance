#include "Enemy.h"

Enemy::Enemy() :
	m_velocity(0.f, 0.f)
{
}

Enemy::Enemy(float x, float y)
{
	setPosition(x, y);
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_shape, states);
}

Enemy::~Enemy()
{
}
