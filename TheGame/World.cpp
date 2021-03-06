#include "World.h"
#include "Definitions.hpp"
#include "Player.h"
#include "Game.h"

World::World(Game * game) :
	m_game(game),
	m_topBorder(sf::PrimitiveType::TriangleFan, 4),
	m_bottomBorder(sf::PrimitiveType::TriangleFan, 4),
	m_lines(sf::PrimitiveType::Lines, 4),
	c_bounds(0.f, 0.f, WORLD_SIZE.x, WORLD_SIZE.y),
	m_color(255, 255, 255, 0)
{
	for (int i = 0; i < 4; i++)
	{
		m_topBorder[i].color = m_color;
		m_bottomBorder[i].color = m_color;
		m_lines[i].color = m_color;
	}
}

sf::FloatRect World::getTopBoundRect() const
{
	return m_topBounds;
}

sf::FloatRect World::getBottomBoundRect() const
{
	return m_bottomBounds;
}

void World::updateBounds(const sf::FloatRect & view)
{
	m_topBorder[0].position = sf::Vector2f(view.left, view.top);
	m_topBorder[1].position = sf::Vector2f(view.left + view.width, view.top);
	m_topBorder[2].position = sf::Vector2f(view.left + view.width, c_bounds.top);
	m_topBorder[3].position = sf::Vector2f(c_bounds.left, c_bounds.top);

	m_topBounds.left = m_topBorder[0].position.x;
	m_topBounds.top = m_topBorder[0].position.y;
	m_topBounds.width = m_topBorder[2].position.x - m_topBorder[0].position.x;
	m_topBounds.height = m_topBorder[2].position.y - m_topBorder[0].position.y;

	m_bottomBorder[0].position = sf::Vector2f(c_bounds.left, c_bounds.top + c_bounds.height);
	m_bottomBorder[1].position = sf::Vector2f(c_bounds.left + c_bounds.width, c_bounds.top + c_bounds.height);
	m_bottomBorder[2].position = sf::Vector2f(view.left + view.width, view.top + view.height);
	m_bottomBorder[3].position = sf::Vector2f(view.left, view.top + view.height);

	m_bottomBounds.left = m_bottomBorder[0].position.x;
	m_bottomBounds.top = m_bottomBorder[0].position.y;
	m_bottomBounds.width = m_bottomBorder[3].position.x - m_bottomBorder[0].position.x;
	m_bottomBounds.height = m_bottomBorder[3].position.y - m_bottomBorder[0].position.y;

	m_lines[0] = sf::Vector2f(c_bounds.left, c_bounds.top);
	m_lines[1] = sf::Vector2f(view.left + view.width, c_bounds.top);
	m_lines[2] = sf::Vector2f(c_bounds.left, c_bounds.top + c_bounds.height);
	m_lines[3] = sf::Vector2f(c_bounds.left + c_bounds.width, c_bounds.top + c_bounds.height);

	for (int i = 0; i < 4; i++)
	{
		m_topBorder[i].color = m_color;
		m_bottomBorder[i].color = m_color;
		m_lines[i].color = m_color;
	}
}

void World::update(sf::Time elapsed, Player* const player, sf::View* const view)
{
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	m_shader->setUniform("windowHeight",
						 static_cast<float>(target.getSize().y));

	target.draw(m_topBorder, states);
	target.draw(m_bottomBorder, states);

	target.draw(m_lines, m_shader);
}

World::~World()
{
}

sf::FloatRect World::getBounds() const
{
	return c_bounds;
}

sf::Vector2<float> World::getCenter()
{
	return sf::Vector2<float>((c_bounds.left + c_bounds.width) / 2,
		(c_bounds.top + c_bounds.height) / 2);
}

void World::setShader(sf::Shader * shader)
{
	m_shader = shader;
}

void World::setBoundsColor(sf::Color color)
{
	m_color = color;

	for (int i = 0; i < 4; i++)
	{
		m_topBorder[i].color = m_color;
		m_bottomBorder[i].color = m_color;
	}
}

sf::Color World::getBoundsColor()
{
	return m_color;
}