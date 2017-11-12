#include "RenderButton.h"



RenderButton::RenderButton()
{
}

RenderButton::RenderButton(sf::FloatRect rect) :
	Button(rect)
{
}


RenderButton::~RenderButton()
{
}

void RenderButton::setRect(const sf::FloatRect & rect)
{
	m_rect = rect;

	setAlignment(m_alignmentX, m_alignmentY);
}

void RenderButton::setTexture(const sf::Texture& texture)
{
	m_sprite.setTexture(texture);
	m_textureRect = m_sprite.getTextureRect();

	setAlignment(Start, Start);
	setFitting(Fit);
}

void RenderButton::setFitting(Fitting fit, float scale)
{
	m_sprite.setTextureRect(m_textureRect);

	sf::FloatRect spriteBounds = m_sprite.getLocalBounds();

	switch (fit)
	{
	case Fit:
	{
		float widthDiff = m_rect.width / spriteBounds.width;
		float heightDiff = m_rect.height / spriteBounds.height;
		float minDiff = std::min(widthDiff, heightDiff);
		m_sprite.setScale(minDiff * scale, minDiff * scale);
		break;
	}
	case Fill:
	{
		float widthDiff = m_rect.width / spriteBounds.width;
		float heightDiff = m_rect.height / spriteBounds.height;
		float maxDiff = std::max(widthDiff, heightDiff);
		m_sprite.setScale(maxDiff * scale, maxDiff * scale);
		break;
	}
	case Stretch:
	{
		float widthDiff = m_rect.width / spriteBounds.width;
		float heightDiff = m_rect.height / spriteBounds.height;
		m_sprite.setScale(widthDiff * scale, heightDiff * scale);
		break;
	}
	default:
		break;
	}
}

void RenderButton::setAlignment(Alignment x, Alignment y)
{
	sf::FloatRect bounds = m_sprite.getLocalBounds();

	sf::Vector2f origin;
	sf::Vector2f position;

	switch (x)
	{
	case Start:
		origin.x   = bounds.left;
		position.x = m_rect.left;
		break;
	case Center:
		origin.x   = bounds.left + bounds.width / 2.f;
		position.x = m_rect.left + m_rect.width / 2.f;
		break;
	case End:
		origin.x   = bounds.left + bounds.width;
		position.x = m_rect.left + m_rect.width;
		break;
	default:
		break;
	}

	switch (y)
	{
	case Start:
		origin.y   = bounds.top;
		position.y = m_rect.top;
		break;
	case Center:
		origin.y   = bounds.top + bounds.height / 2.f;
		position.y = m_rect.top + m_rect.height / 2.f;
		break;
	case End:
		origin.y   = bounds.top + bounds.height;
		position.y = m_rect.top + m_rect.height;
		break;
	default:
		break;
	}

	m_sprite.setOrigin(origin);
	m_sprite.setPosition(position);

	m_alignmentX = x;
	m_alignmentY = y;
}

void RenderButton::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}
