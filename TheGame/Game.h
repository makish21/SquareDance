#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <stack>
#include <assert.h>

#include "AcceptanceBackground.h"
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

struct SharedContext
{
	SharedContext(FileManager*  const fm,
				  sf::View*     const gv,
				  GameObjects*  const go,
				  EnemySpawner* const es,
				  Player*       const p,
				  World*        const w,
				  Background*   const b,
				  sf::Time*     const bt) :
		fileManager(fm),
		gameView(gv),
		objects(go),
		enemySpawner(es),
		player(p),
		world(w),
		background(b),
		bestTime(bt)
	{
	};

	FileManager*   const fileManager;
	sf::View*      const gameView;
	GameObjects*   const objects;
	EnemySpawner*  const enemySpawner;
	Player*        const player;
	World*         const world;
	Background*    const background;

	sf::Time*      const bestTime;
};

class Game
{
	friend class DebugOverlay;
public:
	Game();
	~Game();

	sf::VideoMode getCurrentVideoMode() const;
	sf::VideoMode getBestVideoMode() const;

	const sf::Vector2f& getDefaultViewSize() const;
	float getViewZoom() const;

	void setViewZoom(float newZoom);

	sf::Vector2i mapCoordsToPixel(const sf::Vector2f& point) const;
	sf::Vector2i mapCoordsToPixel(const sf::Vector2f& point, const sf::View& view) const;
	sf::Vector2f mapPixelToCoords(const sf::Vector2i& point) const;
	sf::Vector2f mapPixelToCoords(const sf::Vector2i& point, const sf::View& view) const;

	void setMusicVolume(float volume);
	float getMusicVolume() const;

	bool hasFocus() const;

	void changeState(GameState* state);
	
	void gameLoop();

	void close();

private:

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
	Background* m_background;
	World       m_world;
	Player      m_player;
	GameObjects m_gameObjects;
	sf::Time    m_bestTime;
	sf::Music   m_music;

	// State members
	GameState* m_state;

	// Engine members
	FileManager  m_fileManager;
	InputHandler m_playerInputHandler;
	EnemySpawner m_enemySpawner;
	sf::Event    m_event;
	sf::Clock    m_clock;
	sf::Time     m_elapsed;

	// Debug info
#ifndef NDEBUG
	DebugOverlay m_debugOverlay;
	std::string  m_stateInfo;

	sf::Clock m_titleClock;
	unsigned int m_updatesCounter;
	unsigned int m_framesCounter;

	unsigned int m_ups;
	unsigned int m_fps;
#endif // _DEBUG
};

