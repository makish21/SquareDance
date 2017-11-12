#pragma once
#include <SFML\Graphics.hpp>
#include <ConvexCollision.h>

#include "Object.hpp"
#include "World.h"
#include "Controllable.hpp"
#include "Collidable.hpp"
#include "Subject.h"

class Player :
	public Object, 
	public Controllable,
	public Subject
{
public:
	Player(float x, float y, float w, float h);
	~Player();

	void setColor(const sf::Color& color);
	const sf::Color& getColor() const;

	sf::FloatRect getRect() const;

	sf::Vector2f getVelocity() const;
	void setVelocity(const sf::Vector2f& velocity);

	virtual void moveLeft();
	virtual void moveRight();
	virtual void stopMovingLeft();
	virtual void stopMovingRight();

	virtual void update(World&, sf::Time elapsed);
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

