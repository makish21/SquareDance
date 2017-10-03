#pragma once
#include <SFML\Graphics.hpp>
#include <CollisionDetection.hpp>

class Collidable
{
public:
	Collidable();
	Collidable(cd::Collision* collision);
	~Collidable();

	bool intersects(const Collidable& other);

	cd::Collision* getCollision() const;
	
protected:
	void setCenter(const sf::Vector2f& position);
	void setArea(float radius);

	void setCollision(cd::Collision* collision);
private:
	sf::Vector2f m_position;
	float m_area;

	cd::Collision* m_collision;
};