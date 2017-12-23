#pragma once
#include "Enemy.h"
#include <ConvexCollision.h>
class DefaultEnemy :
	public Enemy
{
public:
	DefaultEnemy(float y, float dirX);

	virtual void update(World&, sf::Time elapsed);

	virtual Enemy* clone();

	~DefaultEnemy();
private:
	cd::Vector2<float>* m_collisionVertices[5];

	void updateCollision();
	bool checkCollisionWithWorld(const World& world);
};

