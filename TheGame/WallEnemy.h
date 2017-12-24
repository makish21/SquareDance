#pragma once
#include "Enemy.h"
class WallEnemy :
	public Enemy
{
public:
	WallEnemy(float top, float bottom, float dir);
	~WallEnemy();

	virtual void update(World& world, sf::Time elapsed);

	virtual Enemy* clone();

private:
	float m_topEdge, m_bottomEdge;
	cd::Vector2<float>* m_collisionVertices[4];

	void updateCollision();
	bool checkCollisionWithWorld(const World& world);
};

