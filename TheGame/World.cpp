#include "World.h"
#include "Definitions.hpp"

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

void World::updateBounds(const sf::FloatRect & view)
{
	m_topBorder[0].position = sf::Vector2f(view.left, view.top);
	m_topBorder[1].position = sf::Vector2f(view.left + view.width, view.top);
	m_topBorder[2].position = sf::Vector2f(view.left + view.width, c_bounds.top);
	m_topBorder[3].position = sf::Vector2f(c_bounds.left, c_bounds.top);

	m_bottomBorder[0].position = sf::Vector2f(c_bounds.left, c_bounds.top + c_bounds.height);
	m_bottomBorder[1].position = sf::Vector2f(c_bounds.left + c_bounds.width, c_bounds.top + c_bounds.height);
	m_bottomBorder[2].position = sf::Vector2f(view.left + view.width, view.top + view.height);
	m_bottomBorder[3].position = sf::Vector2f(view.left, view.top + view.height);

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

	if (sf::Shader::isAvailable())
	{
		float windowHeight = static_cast<float>(m_game->getWindow()->getSize().y);
		m_shader->setUniform("windowHeight", windowHeight);
	}
}

void World::update(sf::Time elapsed)
{
	if (sf::Shader::isAvailable())
	{
		sf::Vector2f playerPosition(m_game->getPlayer()->getPosition());
		sf::View* gameView = m_game->getView();
		sf::Vector2f shaderCenter(m_game->getWindow()->mapCoordsToPixel(playerPosition, *gameView));

		float radius = RADIAL_SHADER_FACTOR * m_game->getWindow()->getSize().x / m_game->getViewZoom();

		m_shader->setUniform("center", shaderCenter);
		m_shader->setUniform("radius", radius);
	}
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
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

	m_shader->setUniform("expand", 0.f);
	m_shader->setUniform("color", sf::Glsl::Vec4(sf::Color::White));
	//m_shader->setUniform("windowHeight", static_cast<float>(m_game->getWindow()->getSize().y));
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