#pragma once
#include <SFML\Graphics.hpp>

class Game;

class DebugOverlay :
	public sf::Drawable
{
public:
	DebugOverlay();
	~DebugOverlay();

	void setGame(const Game* const game);
	void setFont(const sf::Font* const font);

	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	const Game* m_game;

	sf::Text m_text;

	std::string m_windowSize;
	std::string m_bitsPerPixel;
	std::string m_antialiasingLevel;
	std::string m_openGLVersion;
	std::string m_shaders;
	std::string m_updatesCounter;
	std::string m_framesCounter;
	std::string m_playerPos;
	std::string m_viewSize;
	std::string m_titleAlpha;
	std::string m_mousePosition;
};

