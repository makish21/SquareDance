#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <stack>

#include "FileManager.h"
#include "World.h"
#include "Player.h"

class GameState;
class World;

class Game
{
public:
	Game();
	~Game();

	sf::RenderTarget* getWindow();

	sf::View* getView();
	const sf::Vector2f& getDefaultViewSize() const;
	float getViewZoom() const;

	void setViewZoom(float newZoom);

	FileManager* getFileManager();
	World* getWorld();
	Player* getPlayer();

	sf::Uint8 getTitleAlpha() const;
	void setTitleAlpha(sf::Uint8 newAlpha);

	sf::Vector2i getPixelMousePosition() const;
	sf::Vector2i getPixelMousePosition(const sf::View& view) const;
	sf::Vector2f getWorldMousePosition() const;
	sf::Vector2f getWorldMousePosition(const sf::View& view) const;

	void pushState(GameState* state);
	void popState();
	void changeState(GameState* state);

	GameState* peekState();

	bool pollEvent(sf::Event& event);
	void handleEvent(sf::Event& event);

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

	sf::VideoMode m_videoMode;
	sf::ContextSettings m_settings;
	sf::RenderWindow m_window;

	float m_aspectRatio;
	sf::Vector2f m_defaultViewSize;
	float m_viewZoom;
	sf::View m_view;

	FileManager m_fileManager;
	World m_world;
	Player m_player;

	sf::Color m_titleColor;
	sf::Text m_title;

	std::stack<GameState*> m_states;

	sf::Event m_event;

	sf::Vector2i m_mousePosition;

	sf::Clock m_clock;
	sf::Time m_elapsed;

	// Debug info
#if defined(_DEBUG) || defined(__ANDROID__)
	sf::Clock m_titleClock;
	unsigned int m_updates;
	unsigned int m_frames;

	std::string m_dBitsPerPixel;
	std::string m_dUpdates;
	std::string m_dFrames;
	std::string m_dPlayerPos;
	std::string m_dViewSize;
	std::string m_dTitleAlpha;
	std::string m_dMousePosition;

	sf::Text m_debugText;
#endif // _DEBUG
};

