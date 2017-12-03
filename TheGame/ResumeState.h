#pragma once
#include "GameState.h"
#include "RenderButton.h"

class ResumeState :
	public GameState
{
public:
	ResumeState(Game* const game,
				const SharedContext& sharedContext,
				sf::Time* const currentTime,
				sf::Text* const stopwatch,
				sf::RenderTexture*  blurredScene,
				sf::RenderTexture*  FBO_A,
				sf::RenderTexture*  FBO_B,
				sf::RectangleShape* blackout,
				sf::Text*     pauseText,
				RenderButton* returnButton,
				RenderButton* closeButton,
				GameState* nextState);
	~ResumeState();

	virtual void clear();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow& window);

private:
	sf::Time m_elapsedTime;

	sf::RenderTexture*  m_blurredScene;
	sf::RenderTexture*  m_FBO_A;
	sf::RenderTexture*  m_FBO_B;
	sf::Sprite          m_blurSprite;
	sf::Shader*         m_blurShader;
	sf::Color           m_blurColor;
	sf::RectangleShape* m_sceneBlackout;
	sf::Text*           m_pauseText;

	RenderButton* m_returnButton;
	RenderButton* m_closeButton;

	sf::Time* const m_currentTime;
	sf::Text* const m_stopwatchText;

	GameState* m_nextState;
};

