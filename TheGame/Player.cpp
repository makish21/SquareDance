#include "Player.h"
#include "Definitions.hpp"

Player::Player(float x, float y, float w, float h) : 
	m_shape(PLAYER_RADIUS, 4),
	m_velocity(0.f, 3.f)
{
	setPosition(x, y);
	m_shape.setFillColor(sf::Color(215, 255, 255));

	setOrigin(m_shape.getRadius(), m_shape.getRadius());

	m_rotationDirection = -1;

	for (int i = 0; i < 4; i++)
	{
		m_collisionVertices[i] = new cd::Vector2<float>();
	}

	setCollision(new cd::ConvexCollision(&m_collisionVertices[0], 4));
	setArea(m_shape.getRadius());
	updateCollision();
}

Player::~Player()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_collisionVertices[i])
		{
			delete m_collisionVertices[i];
			m_collisionVertices[i] = nullptr;
		}
	}
}

void Player::setColor(const sf::Color & color)
{
	m_shape.setFillColor(color);
}

const sf::Color & Player::getColor() const
{
	return m_shape.getFillColor();
}

sf::FloatRect Player::getRect() const
{
	sf::Vector2f point = getTransform().transformPoint(m_shape.getPoint(0));

	float minX = point.x;
	float maxX = minX;
	float minY = point.y;
	float maxY = minY;

	for (size_t i = 0; i < 4; i++)
	{
		point = getTransform().transformPoint(m_shape.getPoint(i));

		minX = std::min(point.x, minX);
		minY = std::min(point.y, minY);
		maxX = std::max(point.x, maxX);
		maxY = std::max(point.y, maxY);
	}

	return sf::FloatRect(minX, minY, maxX - minX, maxY - minY);
}

sf::Vector2f Player::getVelocity() const
{
	return m_velocity;
}

void Player::setVelocity(const sf::Vector2f & velocity)
{
	m_velocity = velocity;
}

void Player::moveLeft()
{
	m_velocity.x -= 2.f;
}

void Player::moveRight()
{
	m_velocity.x += 2.f;
}

void Player::stopMovingLeft()
{
	m_velocity.x += 2.f;
}

void Player::stopMovingRight()
{
	m_velocity.x -= 2.f;
}

void Player::update(World& world, sf::Time elapsed)
{
	move(m_velocity * static_cast<float>(elapsed.asMilliseconds()) / 11.5f);

	checkCollisionWithWorld(world);
	rotateComputation(world);
	
	updateCollision();

	if (!m_life)
	{
		m_shape.setFillColor(sf::Color::Red);
	}
	else
	{
		m_shape.setFillColor(sf::Color(215, 255, 255));
	}
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_shape, states);
}

void Player::updateCollision()
{
	setCenter(getPosition());

	for (size_t i = 0; i < 4; i++)
	{
		sf::Vector2f vertex = getTransform().transformPoint(m_shape.getPoint(i));
		*m_collisionVertices[i] = cd::Vector2<float>(vertex.x, vertex.y);
	}
}

bool Player::checkCollisionWithWorld(const World& world)
{
	sf::FloatRect rect = getRect();
	sf::FloatRect intersection;

	if (world.getBounds().intersects(rect, intersection))
	{
		if (intersection == rect)
		{
			return false;
		}
		else
		{
			float widthDiff = rect.width - intersection.width;
			float heightDiff = rect.height - intersection.height;

			float xDiff = rect.left - world.getBounds().left;
			float yDiff = rect.top - world.getBounds().top;

			int dirX = xDiff != 0.f ? static_cast<int>(-xDiff / std::abs(-xDiff)) : 0;
			int dirY = yDiff != 0.f ? static_cast<int>(-yDiff / std::abs(-yDiff)) : 0;

			if (heightDiff > 0.f)
			{
				m_velocity.y = -m_velocity.y;

				if (m_velocity.x != 0.f && dirY != 0.f)
				{
					m_rotationDirection = static_cast<int>(m_velocity.x / std::abs(m_velocity.x)) * -dirY;
				}

				GameEvent event;
				event.type = GameEvent::PlayerBounced;
				event.player.x = getPosition().x;
				event.player.y = getPosition().y;
				notify(event);
			}

			if (heightDiff != 0.f || widthDiff != 0.f)
			{
				move(widthDiff * dirX, (heightDiff * 2.f) * dirY);
			}

			return true;
		}
	}

	return true;
}

void Player::rotateComputation(World& world)
{
	setRotation(m_rotationDirection * static_cast<int>((m_velocity.y / abs(m_velocity.y))) * 
		(getPosition().y - (world.getBounds().top + 20 / 2)) / 
				(world.getBounds().height - 20) * 180 + 45);
}
