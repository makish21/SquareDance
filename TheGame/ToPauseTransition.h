#pragma once
#include "GameState.h"
#include "RenderButton.h"

class ToPauseTransition :
	public GameState
{
public:
	ToPauseTransition(Game* const game,
					  const SharedContext& sharedContext);
	~ToPauseTransition();

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
};

