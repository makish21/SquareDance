#include "World.h"

#define BOUNDS_H 280

World::World(sf::FloatRect rect) :
	m_topBorder(sf::PrimitiveType::TriangleFan, 4),
	m_bottomBorder(sf::PrimitiveType::TriangleFan, 4),
	m_lines(sf::PrimitiveType::Lines, 4),
	m_bounds(rect.left, 
			 rect.height / 2 + rect.top - BOUNDS_H / 2,
			 rect.width,
			 BOUNDS_H)
{
	m_topBorder[0].position = sf::Vector2f(rect.left, rect.top);
	m_topBorder[1].position = sf::Vector2f(rect.left + rect.width, rect.top);
	m_topBorder[2].position = sf::Vector2f(rect.left + rect.width, m_bounds.top);
	m_topBorder[3].position = sf::Vector2f(m_bounds.left, m_bounds.top);

	m_bottomBorder[0].position = sf::Vector2f(m_bounds.left, m_bounds.top + m_bounds.height);
	m_bottomBorder[1].position = sf::Vector2f(m_bounds.left + m_bounds.width, m_bounds.top + m_bounds.height);
	m_bottomBorder[2].position = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
	m_bottomBorder[3].position = sf::Vector2f(rect.left, rect.top + rect.height);

	m_lines[0] = sf::Vector2f(m_bounds.left, m_bounds.top);
	m_lines[1] = sf::Vector2f(rect.left + rect.width, m_bounds.top);
	m_lines[2] = sf::Vector2f(m_bounds.left, m_bounds.top + m_bounds.height);
	m_lines[3] = sf::Vector2f(m_bounds.left + m_bounds.width, m_bounds.top + m_bounds.height);

	m_color = sf::Color(255, 255, 255, 0);

	for (int i = 0; i < 4; i++)
	{
		m_topBorder[i].color = m_color;
		m_bottomBorder[i].color = m_color;
		m_lines[i].color = m_color;
	}

	m_shader.loadFromFile("RadialGradient.vert", "RadialGradient.frag");
	m_shader.setUniform("color", sf::Glsl::Vec4(sf::Color::White));
	m_shader.setUniform("expand", 0.f);
	m_shader.setUniform("radius", 50.f);
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_topBorder, states);
	target.draw(m_bottomBorder, states);

	target.draw(m_lines, &m_shader);
}

World::~World()
{
}																	

sf::FloatRect World::getBounds() const
{
	return m_bounds;
}

sf::Vector2<float> World::getCenter()
{
	return sf::Vector2<float>((m_bounds.left + m_bounds.width) / 2, (m_bounds.top + m_bounds.height) / 2);
}

void World::setAlpha(sf::Uint8 alpha)
{
	m_color.a = alpha;

	for (int i = 0; i < 4; i++)
	{
		m_topBorder[i].color = m_color;
		m_bottomBorder[i].color = m_color;
	}
}

sf::Uint8 World::getAlpha()
{
	return m_color.a;
}

void World::setPlayerPosition(const sf::Vector2f& playerPos)
{
	m_shader.setUniform("center", playerPos);
}

void World::setWindowHeight(unsigned int height)
{
	m_shader.setUniform("windowHeight", static_cast<float>(height));
}