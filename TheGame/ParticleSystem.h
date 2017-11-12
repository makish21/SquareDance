#pragma once
#include <vector>
#include <SFML\Graphics.hpp>
#include "Vector2.hpp"

struct Particle
{
	sf::Vector2f velocity;
	sf::Time lifeTime;
};

class ParticleSystem :
	public sf::Drawable,
	public sf::Transformable
{
public:
	ParticleSystem(sf::Uint16 count, const sf::Color& color, sf::Time lifetime);
	~ParticleSystem();

	void setEmitter(sf::Vector2f position);
	void setDirection(sf::Vector2f direction);

	void resetParticles();

	void update(sf::Time elapsed);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Time m_elapsedTime;
	//sf::Clock m_clock;

	std::vector<Particle> m_particles;
	std::vector<sf::VertexArray> m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
	sf::Vector2f m_direction;
	sf::Color m_color;
};

