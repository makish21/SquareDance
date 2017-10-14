#include "DebugOverlay.h"
#include "Game.h"

#include <SFML\OpenGL.hpp>

DebugOverlay::DebugOverlay()
{
	m_updatesCounter = "Updates: 0\n";
	m_framesCounter = "Frames: 0\n";
}

DebugOverlay::~DebugOverlay()
{
}

void DebugOverlay::setGame(const Game * const game)
{
	m_game = game;
}

void DebugOverlay::setFont(const sf::Font * const font)
{
	m_text.setFont(*font);
	m_text.setCharacterSize(15);
	m_text.setPosition(10.f, 10.f);
	m_text.setFillColor(sf::Color::White);
}

void DebugOverlay::update()
{
	m_windowSize = "Window size: w: " + std::to_string(m_game->m_currentVideoMode.width) + " h: " +
		std::to_string(m_game->m_currentVideoMode.height) + '\n';

	m_bitsPerPixel = "Bits per pixel: " + std::to_string(m_game->m_currentVideoMode.bitsPerPixel) + '\n';

	m_antialiasingLevel = "Anti-aliasing: " + std::to_string(m_game->m_settings.antialiasingLevel) + "x\n";

	//m_openGLVersion = "OpenGL: v" + std::string((char*)(glGetString(GL_VERSION))) + '\n';

	m_shaders = std::string("Is shaders available: ") + (sf::Shader::isAvailable() ? "true" : "false") + '\n';

	m_updatesCounter = "Updates: " + std::to_string(m_game->m_ups) + '\n';

	m_framesCounter = "Frames: " + std::to_string(m_game->m_fps) + '\n';

	m_playerPos = "Player position: x: " + std::to_string(static_cast<int>(m_game->m_player.getPosition().x)) +
		" y: " + std::to_string(static_cast<int>(m_game->m_player.getPosition().y)) + '\n';

	m_viewSize = "View size: x: " + std::to_string(static_cast<int>(m_game->m_view.getSize().x)) +
		" y: " + std::to_string(static_cast<int>(m_game->m_view.getSize().y)) + '\n';

	m_titleAlpha = "Title alpha: " + std::to_string(m_game->m_titleColor.a) + '\n';

	m_mousePosition = "Mouse position: x: " + std::to_string(static_cast<int>(m_game->getWorldMousePosition(m_game->m_view).x)) +
		" y: " + std::to_string(static_cast<int>(m_game->getWorldMousePosition(m_game->m_view).y)) + '\n';

	m_text.setString(m_windowSize +
					 m_bitsPerPixel +
					 m_antialiasingLevel +
					 m_openGLVersion +
					 m_shaders +
					 m_updatesCounter +
					 m_framesCounter +
					 m_playerPos +
					 m_viewSize +
					 m_titleAlpha +
					 m_mousePosition);
}

void DebugOverlay::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.setView(target.getDefaultView());
	target.draw(m_text);
}
