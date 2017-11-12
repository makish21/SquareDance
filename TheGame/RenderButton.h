#pragma once
#include "Button.h"

enum Alignment
{
	Start,
	Center,
	End
};

enum Fitting
{
	Fit,
	Fill,
	Stretch,
};

class RenderButton :
	public Button,
	public sf::Drawable
{
public:
	RenderButton();
	RenderButton(sf::FloatRect rect);
	~RenderButton();

	virtual void setRect(const sf::FloatRect& rect);

	void setTexture(const sf::Texture& texture);

	void setFitting(Fitting fit, float scale = 1.f);
	void setAlignment(Alignment x, Alignment y);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite m_sprite;
	sf::IntRect m_textureRect;

	Alignment m_alignmentX;
	Alignment m_alignmentY;
};

