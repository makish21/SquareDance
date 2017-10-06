#pragma once
#include <vector>
#include <SFML\Graphics.hpp>

struct Particle
{
	sf::Vector2f velocity;
	float lifeTime;
};

class ParticleSystem :
	public sf::Drawable,
	public sf::Transformable
{
public:
	ParticleSystem(sf::Uint16 count);
	~ParticleSystem();

	void setEmitter(sf::Vector2f position);
	void setDirection(sf::Vector2f direction);

	void resetParticles();

	void update(float elapsed);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Particle> m_particles;
	std::vector<sf::VertexArray> m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
	sf::Vector2f m_direction;
};

