#include "ParticleSystem.h"
#include "Definitions.hpp"
#include <complex>
#include <SFML\OpenGL.hpp>

ParticleSystem::ParticleSystem(sf::Uint16 count, unsigned int windowWidth) :
	m_windowWidth(windowWidth),
	m_particles(count),
	m_vertices(3),
	m_lifetime(sf::seconds(3.f)),
	m_emitter(0.f, 0.f)
{
	for (auto i = m_vertices.begin(); i != m_vertices.end(); i++)
	{
		i->resize(count / 3);
		i->setPrimitiveType(sf::Points);

		for (size_t j = 0; j < i->getVertexCount(); j++)
		{
			(*i)[j].color = sf::Color(255, 255, 255, 0);
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
	for (std::size_t i = 0; i < m_particles.size(); i++)
	{
		Particle& p = m_particles[i];
		p.lifeTime -= elapsed.asSeconds();

		float ratio = p.lifeTime / m_lifetime.asSeconds();

		size_t j = i / (m_particles.size() / m_vertices.size());
		size_t k = i % (m_particles.size() / m_vertices.size());

		p.velocity += sf::Vector2f(0.f, 0.5f) * (m_clock.getElapsedTime().asSeconds() / 2.f);
		m_vertices[j][k].position += p.velocity * (static_cast<float>(elapsed.asMilliseconds()) / 22.f);
		//m_vertices[j][k].color.a = static_cast<sf::Uint8>(ratio * 255);
	}
}

void ParticleSystem::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;

	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		float pointSize = static_cast<float>((i + 1.f) * (POINT_SIZE_FACTOR * m_windowWidth));
		glPointSize(pointSize);
		target.draw(m_vertices[i]);
	}
}

void ParticleSystem::resetParticles()
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		float angle = std::rand() % 360 - 180.f;
		float speed = (std::rand() % 5000) / 1000.f /*+ 1.f*/;

		m_particles[i].velocity = sf::Vector2f(std::cos(angle) * speed,
											   std::sin(angle) * speed) + (m_direction * 2.f);
		m_particles[i].lifeTime = std::rand() % 1 + 1.f;

		size_t j = i / (m_particles.size() / m_vertices.size());
		size_t k = i % (m_particles.size() / m_vertices.size());
		m_vertices[j][k].position = sf::Vector2f(m_emitter.x - 5.f + std::rand() % 10,
												 m_emitter.y - 5.f + std::rand() % 10);
		m_vertices[j][k].color = sf::Color(255, 255, 255, 255);
	}

	m_clock.restart();
}
