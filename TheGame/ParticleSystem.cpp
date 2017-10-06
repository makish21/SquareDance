#include "ParticleSystem.h"
#include "Definitions.hpp"
#include <SFML\OpenGL.hpp>

ParticleSystem::ParticleSystem(sf::Uint16 count) :
	m_particles(count),
	m_vertices(3),
	m_lifetime(sf::seconds(3.f)),
	m_emitter(0.f, 0.f)
{
	for (auto i = m_vertices.begin(); i != m_vertices.end(); i++)
	{
		i->resize(count / 3);
		i->setPrimitiveType(sf::Points);
	}
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::setEmitter(sf::Vector2f position)
{
	m_emitter = position;
}

void ParticleSystem::setDirection(sf::Vector2f direction)
{
	m_direction = direction;
}

void ParticleSystem::update(float elapsed)
{
	for (std::size_t i = 0; i < m_particles.size(); i++)
	{
			Particle& p = m_particles[i];
			p.lifeTime -= elapsed;

			float ratio = p.lifeTime / m_lifetime.asSeconds();

			size_t j = i / (m_particles.size() / m_vertices.size());
			size_t k = i % (m_particles.size() / m_vertices.size());
			m_vertices[j][k].position += p.velocity * elapsed;
			m_vertices[j][k].color.a = static_cast<sf::Uint8>(ratio * 255);
	}
}

void ParticleSystem::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;

	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		glPointSize(static_cast<float>(i) + 1.f);
		target.draw(m_vertices[i]);
	}
}

void ParticleSystem::resetParticles()
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		float angle = std::atan2f(m_direction.y, m_direction.x) - 0.8f + std::rand() % 8 / 10.f;
		float speed = (std::rand() % 2000) / 1000.f + 1.f;

		m_particles[i].velocity = sf::Vector2f(std::cos(angle) * speed,
											   std::sin(angle) * speed);
		m_particles[i].lifeTime = std::rand() % 1 + 1;

		size_t j = i / (m_particles.size() / m_vertices.size());
		size_t k = i % (m_particles.size() / m_vertices.size());
		m_vertices[j][k].position = sf::Vector2f(m_emitter.x - 5.f + std::rand() % 10,
												 m_emitter.y - 5.f + std::rand() % 10);
	}
}
