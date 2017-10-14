#include "Background.h"
#include "Definitions.hpp"
#include <complex>

Background::Background(const sf::Vector2u & windowSize) :
	m_drawableArea(std::sqrt(std::pow(windowSize.x, 2) + std::pow(windowSize.y, 2))),
	m_color(6, 6, 28),
	m_smallRect(sf::Vector2f(m_drawableArea, m_drawableArea) / 7.f * 0.2f),
	m_mediumRect(sf::Vector2f(m_drawableArea, m_drawableArea) / 7.f * 0.35f),
	m_largeRect(sf::Vector2f(m_drawableArea, m_drawableArea) / 7.f * 0.5f)
{
	setOrigin(m_drawableArea / 2.f, m_drawableArea / 2.f);
	setPosition(windowSize.x / 2.f, windowSize.y / 2.f);

	m_smallRect.setOrigin(m_smallRect.getSize() / 2.f);
	m_mediumRect.setOrigin(m_mediumRect.getSize() / 2.f);
	m_largeRect.setOrigin(m_largeRect.getSize() / 2.f);

	m_smallRect.setFillColor(m_color);
	m_mediumRect.setFillColor(m_color);
	m_largeRect.setFillColor(m_color);
}

Background::~Background()
{
}

void Background::update(sf::Time elapsed)
{
	float elapsedTime = m_clock.getElapsedTime().asSeconds();

	rotate(elapsed.asSeconds() * 10.f);

	if (elapsedTime < 2.f)
	{
		if (elapsedTime >= 0.f && elapsedTime <= 0.5f)
		{
			float factor = 1.f - std::sin((elapsedTime - 0.f) / 0.5f * PI / 2.f);

			m_largeRect.setRotation((0.f - 90.f) * factor + 90.f);
			m_smallRect.setRotation((0.f + 90.f) * factor - 90.f);
		}

		if (elapsedTime >= 1.f && elapsedTime <= 1.5f)
		{
			float factor = 1.f - std::sin((elapsedTime - 1.f) / 0.5f * PI / 2.f);

			m_mediumRect.setRotation((0.f + 90.f) * factor - 90.f);
			m_smallRect.setRotation((0.f - 90.f) * factor + 90.f);
		}
	}
	else
	{
		m_clock.restart();
	}
}

void Background::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			sf::Transform transform = sf::Transform::Identity;
			transform.translate(sf::Vector2f(m_drawableArea / 8.f * i, m_drawableArea / 8.f * j));
			states.transform = getTransform() * transform;

			if (i % 2 == 0 && j % 2 == 0)
			{
				target.draw(m_largeRect, states);
			}
			else if ((i + 1) % 2 == 0 && (j + 1) % 2 == 0)
			{
				target.draw(m_smallRect, states);
			}
			else
			{
				target.draw(m_mediumRect, states);
			}
		}
	}
}
