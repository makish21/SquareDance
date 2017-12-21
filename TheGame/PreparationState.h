#pragma once
#include "GameState.h"
#include "RenderButton.h"
class PreparationState :
	public GameState
{
public:
	PreparationState(Game* const game,
					 const SharedContext& sharedContext,
					 sf::Time* const currentTime,
					 sf::Text* const stopwatch);
	~PreparationState();

	virtual void clear();

	virtual void handleInput(const sf::Event& event);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderWindow&);

private:
	sf::Time m_stateTime;

	sf::Time* const m_currentTime;
	sf::Text* const m_stopwatchText;

	RenderButton* const m_pauseButton;

	sf::RenderTexture* const m_effect;
	sf::Time* const m_effectTime;
	sf::Sprite m_sprite;
};

