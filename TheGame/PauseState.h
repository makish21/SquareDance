#pragma once
#include "GameState.h"
#include "RenderButton.h"

class PauseState :
	public GameState
{
public:
	PauseState(Game* const game,
			   const SharedContext& sharedContext,
			   sf::RenderTexture*  blurredScene,
			   sf::RenderTexture*  FBO_A,
			   sf::RenderTexture*  FBO_B,
			   sf::RectangleShape* blackout,
			   sf::Text*     pauseText,
			   RenderButton* returnButton,
			   RenderButton* closeButton);
	~PauseState();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow& window);

private:
	sf::RenderTexture*  m_blurredScene;
	sf::RenderTexture*  m_FBO_A;
	sf::RenderTexture*  m_FBO_B;
	sf::Sprite          m_blurSprite;
	sf::Shader*         m_blurShader;
	sf::RectangleShape* m_sceneBlackout;
	sf::Text*           m_pauseText;

	RenderButton* m_returnButton;
	RenderButton* m_closeButton;
};

