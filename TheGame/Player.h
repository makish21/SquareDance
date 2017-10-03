#pragma once
#include <SFML\Graphics.hpp>
#include <ConvexCollision.h>

#include "Object.hpp"
#include "World.h"
#include "Controllable.hpp"
#include "Collidable.hpp"

class Player :
	public Object, 
	public Controllable
{
public:
	Player(float x, float y, float w, float h);
	~Player();

	sf::FloatRect getRect() const;

	virtual void left();
	virtual void right();

	virtual void update(World&, float time);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void updateCollision();
	bool checkCollisionWithWorld(const World& world);

	void rotateComputation(World&);
	
	cd::Vector2<float>* m_collisionVertices[4];
	sf::Vector2f m_velocity;
	int m_rotationDirection;
	sf::CircleShape m_shape;
};

