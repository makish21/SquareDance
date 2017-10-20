#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <stack>

#include "Background.h"
#include "DebugOverlay.h"
#include "FileManager.h"
#include "World.h"
#include "Player.h"
#include "InputHandler.h"
#include "EnemySpawner.h"
#include "Definitions.hpp"

using GameObjects = std::list<Object*>;

class GameState;
class World;

class Game
{
	friend class DebugOverlay;
public:
	Game();
	~Game();

	sf::RenderWindow* getWindow();

	const sf::Vector2f& getDefaultViewSize() const;
	float getViewZoom() const;

	void setViewZoom(float newZoom);

	sf::Color getTitleColor() const;
	void setTitleColor(sf::Color color);

	sf::Vector2i getPixelMousePosition() const;
	sf::Vector2i getPixelMousePosition(const sf::View& view) const;
	sf::Vector2f getWorldMousePosition() const;
	sf::Vector2f getWorldMousePosition(const sf::View& view) const;

	void setMusicVolume(float volume);
	float getMusicVolume() const;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);

	GameState* peekState();

	void gameLoop();

	void close();

private:
	void clearStates();

	void updateRender(sf::VideoMode videoMode);
	void updateRender(unsigned int width, unsigned int height, unsigned int bitsPerPixel = 32U);

	void handleInput();
	void update();
	void render();

	void restartClock();

	// Window members
	sf::VideoMode       m_bestVideoMode;
	sf::VideoMode       m_currentVideoMode;
	sf::ContextSettings m_settings;
	sf::RenderWindow    m_window;

	// View members
	float        m_aspectRatio;
	sf::Vector2f m_defaultViewSize;
	float        m_viewZoom;
	sf::View     m_view;

	// Game members
	Background  m_background;
	World       m_world;
	Player      m_player;
	GameObjects m_gameObjects;
	sf::Color   m_titleColor;
	sf::Text    m_title;
	sf::Music   m_music;

	// State members
	std::stack<GameState*> m_states;

	// Engine members
	FileManager  m_fileManager;
	InputHandler m_playerInputHandler;
	EnemySpawner m_enemySpawner;
	sf::Event    m_event;
	sf::Vector2i m_mousePosition;
	sf::Clock    m_clock;
	sf::Time     m_elapsed;

	// Debug info
#if defined(_DEBUG) || defined(__ANDROID__)
	DebugOverlay m_debugOverlay;

	sf::Clock m_titleClock;
	unsigned int m_updatesCounter;
	unsigned int m_framesCounter;

	unsigned int m_ups;
	unsigned int m_fps;
#endif // _DEBUG
};

