#pragma once
#include <SFML\Graphics.hpp>

class Player;
class Game;

class World :
	public sf::Drawable
{
public:
	World(Game* game);
	//World(sf::FloatRect window);

	sf::FloatRect getBounds() const;
	sf::Vector2<float> getCenter();

	void setShader(sf::Shader* shader);

	void setBoundsColor(sf::Color color);
	sf::Color getBoundsColor();

	void updateBounds(const sf::FloatRect& view);
	void update(sf::Time elapsed, Player* const player, sf::View* const view);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	~World();
private:
	Game* m_game;

	const sf::FloatRect c_bounds;

	sf::VertexArray m_topBorder;
	sf::VertexArray m_bottomBorder;

	sf::VertexArray m_lines;
	sf::Shader* m_shader;

	sf::Color m_color;
};

