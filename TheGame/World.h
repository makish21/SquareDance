#pragma once
#include <SFML\Graphics.hpp>
class World :
	public sf::Drawable
{
public:
	World(sf::FloatRect window);

	sf::FloatRect getBounds() const;
	sf::Vector2<float> getCenter();

	void setAlpha(sf::Uint8 alpha);
	sf::Uint8 getAlpha();

	void setPlayerPosition(const sf::Vector2f& playerPos);
	void setWindowHeight(unsigned int height);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	~World();
private:
	sf::FloatRect m_bounds;
	sf::VertexArray m_topBorder;
	sf::VertexArray m_bottomBorder;

	sf::VertexArray m_lines;
	sf::Shader m_shader;

	sf::Color m_color;
};

