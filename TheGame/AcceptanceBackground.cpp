#include <complex>
#include "Animations.h"
#include "AcceptanceBackground.h"

AcceptanceBackground::AcceptanceBackground(const sf::Vector2u & windowSize) :
	Background(windowSize),
	m_smallRect(sf::Vector2f(m_drawableArea, m_drawableArea) / BACKGROUND_PARTITION_NUM * 0.2f),
	m_mediumRect(sf::Vector2f(m_drawableArea, m_drawableArea) / BACKGROUND_PARTITION_NUM * 0.35f),
	m_largeRect(sf::Vector2f(m_drawableArea, m_drawableArea) / BACKGROUND_PARTITION_NUM * 0.5f),
	m_action(Action::Medium),
	m_inAction(false)
{
	setOrigin(m_drawableArea / 2.f, m_drawableArea / 2.f);
	setPosition(windowSize.x / 2.f, windowSize.y / 2.f);

	m_smallRect.setOrigin(m_smallRect.getSize() / 2.f);
	m_mediumRect.setOrigin(m_mediumRect.getSize() / 2.f);
	m_largeRect.setOrigin(m_largeRect.getSize() / 2.f);

	m_smallRect.setScale(0.9f, 0.9f);
	m_mediumRect.setScale(0.9f, 0.9f);
	m_largeRect.setScale(0.9f, 0.9f);

	m_smallRect.setFillColor(m_color);
	m_mediumRect.setFillColor(m_color);
	m_largeRect.setFillColor(m_color);
}

AcceptanceBackground::~AcceptanceBackground()
{
}

void AcceptanceBackground::setColor(sf::Color color)
{
	m_color = color;

	m_smallRect.setFillColor(color);
	m_mediumRect.setFillColor(color);
	m_largeRect.setFillColor(color);
}

void AcceptanceBackground::onNotify(const GameEvent & event)
{
	switch (event.type)
	{
	case GameEvent::PlayerBounced:
		m_pulseTime = sf::Time::Zero;

		m_actionTime = sf::Time::Zero;
		m_inAction = true;

		// Next action
		m_action++;
		break;

	default:
		break;
	}
}

void AcceptanceBackground::update(sf::Time elapsed)
{
	rotate(elapsed.asSeconds() * 10.f);

	m_pulseTime += elapsed;

	float rectScaleFactor = pulse(m_pulseTime, sf::seconds(1.9f), 0.85f, 0.15f);

	m_smallRect.setScale(rectScaleFactor, rectScaleFactor);
	m_mediumRect.setScale(rectScaleFactor, rectScaleFactor);
	m_largeRect.setScale(rectScaleFactor, rectScaleFactor);

	if (m_inAction)
	{
		m_actionTime += elapsed;

		if (m_actionTime.asSeconds() <= 0.4f)
		{
			float rotationFactor = 1.f - std::sin((m_actionTime.asSeconds()) / 0.4f * PI / 2.f);

			switch (m_action)
			{
			case Action::Large:
				m_largeRect.setRotation((0.f - 90.f) * rotationFactor + 90.f);
				m_smallRect.setRotation((0.f + 90.f) * rotationFactor - 90.f);
				break;

			case Action::Medium:
				m_mediumRect.setRotation((0.f + 90.f) * rotationFactor - 90.f);
				m_smallRect.setRotation((0.f - 90.f) * rotationFactor + 90.f);
				break;

			default:
				break;
			}
		}
		else
		{
			m_actionTime = sf::Time::Zero;
			m_inAction = false;
		}
	}
}

void AcceptanceBackground::draw(sf::RenderTarget & target, sf::RenderStates states) const
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

// Increase enum
inline Action & operator++(Action & action, int)
{
	const int count = static_cast<int>(Action::Count);
	const int i = static_cast<int>(action);
	action = static_cast<Action>((i + 1) % count);
	return action;
}
