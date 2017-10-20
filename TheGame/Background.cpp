#include <complex>
#include "Background.h"
#include "Definitions.hpp"

Background::Background(const sf::Vector2u & windowSize) :
	m_drawableArea(static_cast<float>(std::sqrt(std::pow(windowSize.x, 2) + std::pow(windowSize.y, 2)))),
	m_color(BACKGROUND_COLOR),
	m_smallRect(sf::Vector2f(m_drawableArea, m_drawableArea) / BACKGROUND_PARTITION_NUM * 0.2f),
	m_mediumRect(sf::Vector2f(m_drawableArea, m_drawableArea) / BACKGROUND_PARTITION_NUM * 0.35f),
	m_largeRect(sf::Vector2f(m_drawableArea, m_drawableArea) / BACKGROUND_PARTITION_NUM * 0.5f)
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

	float transformScaleFactor = std::sin(elapsedTime * PI) * 0.1f + 0.9f;

	setScale(transformScaleFactor, transformScaleFactor);

	float rectScaleFactor = std::sin(elapsedTime * PI * 2.f) * 0.1f + 0.9f;

	m_smallRect.setScale(rectScaleFactor, rectScaleFactor);
	m_mediumRect.setScale(rectScaleFactor, rectScaleFactor);
	m_largeRect.setScale(rectScaleFactor, rectScaleFactor);

	if (elapsedTime < 2.f)
	{
		if (elapsedTime >= 0.f && elapsedTime <= 0.5f)
		{
			float rotationFactor = 1.f - std::sin((elapsedTime - 0.f) / 0.5f * PI / 2.f);

			m_largeRect.setRotation((0.f - 90.f) * rotationFactor + 90.f);
			m_smallRect.setRotation((0.f + 90.f) * rotationFactor - 90.f);
		}

		if (elapsedTime >= 1.f && elapsedTime <= 1.5f)
		{
			float rotationFactor = 1.f - std::sin((elapsedTime - 1.f) / 0.5f * PI / 2.f);

			m_mediumRect.setRotation((0.f + 90.f) * rotationFactor - 90.f);
			m_smallRect.setRotation((0.f - 90.f) * rotationFactor + 90.f);
		}
	}
	else
	{
		m_clock.restart();
	}
}

void Background::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (int i = 0; i <= 7; i++)
	{
		for (int j = 0; j <= 7; j++)
		{
			sf::Transform transform = sf::Transform::Identity;
			transform.translate(sf::Vector2f(m_drawableArea / (BACKGROUND_PARTITION_NUM + 1.f) * i,
											 m_drawableArea / (BACKGROUND_PARTITION_NUM + 1.f) * j));
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
