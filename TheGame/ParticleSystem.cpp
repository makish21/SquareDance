#include "ParticleSystem.h"
#include "Definitions.hpp"
#include "Animations.h"
#include <complex>
#include <SFML\OpenGL.hpp>

ParticleSystem::ParticleSystem(sf::Uint16 count, const sf::Color& color, sf::Time lifetime) :
	m_particles(count / VERTICES_DIVERSITY_COUNT * VERTICES_DIVERSITY_COUNT),
	m_vertices(VERTICES_DIVERSITY_COUNT),
	m_lifetime(lifetime),
	m_color(color),
	m_emitter(0.f, 0.f)
{
	for (auto i = m_vertices.begin(); i != m_vertices.end(); i++)
	{
		i->resize(count / VERTICES_DIVERSITY_COUNT);
		i->setPrimitiveType(sf::Points);

		for (size_t j = 0; j < i->getVertexCount(); j++)
		{
			(*i)[j].color = color;
		}
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

void ParticleSystem::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	for (std::size_t i = 0; i < m_particles.size(); i++)
	{
		Particle& p = m_particles[i];
		p.lifeTime -= elapsed;

		if (p.lifeTime.asSeconds() < 1.f && p.lifeTime.asSeconds() > 0.f)
		{
			m_color.a = transfer(p.lifeTime, sf::seconds(1.f),
								 sf::Uint8(0), sf::Uint8(255));
		}

		size_t j = i / (m_particles.size() / m_vertices.size());
		size_t k = i % (m_particles.size() / m_vertices.size());

		p.velocity += sf::Vector2f(0.f, 0.5f) * m_elapsedTime.asSeconds() / 2.f;
		m_vertices[j][k].position += p.velocity * (static_cast<float>(elapsed.asMilliseconds()) / 22.f);
		m_vertices[j][k].color = m_color;
	}
}

void ParticleSystem::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;

	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		float pointSize = static_cast<float>((i + 1.f) * (POINT_SIZE_FACTOR * target.getSize().x));
		glPointSize(pointSize);
		target.draw(m_vertices[i]);
	}
}

void ParticleSystem::resetParticles()
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		float angle = std::rand() % 360 - 180.f;
		float speed = (std::rand() % 5000) / 1000.f;

		m_particles[i].velocity = sf::Vector2f(std::cos(angle) * speed,
											   std::sin(angle) * speed) + (m_direction * 2.f);
		m_particles[i].lifeTime = sf::microseconds(m_lifetime.asMicroseconds() - 
												   std::rand() % (m_lifetime.asMicroseconds() / 2));

		size_t j = i / (m_particles.size() / m_vertices.size());
		size_t k = i % (m_particles.size() / m_vertices.size());
		m_vertices[j][k].position = sf::Vector2f(m_emitter.x - 5.f + std::rand() % 10,
												 m_emitter.y - 5.f + std::rand() % 10);
		m_vertices[j][k].color = m_color;
	}
}
