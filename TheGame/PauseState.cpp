#include "PauseState.h"
#include "PreparationState.h"
#include "MainMenuState.h"
#include "ResumeState.h"

PauseState::PauseState(Game * const game,
					   const SharedContext& sharedContext,
					   sf::Time* const currentTime,
					   sf::Text* const stopwatch,
					   sf::RenderTexture*  blurredScene,
					   sf::RenderTexture*  FBO_A,
					   sf::RenderTexture*  FBO_B,
					   sf::RectangleShape* blackout,
					   sf::Text*     pauseText,
					   RenderButton* returnButton,
					   RenderButton* closeButton) :
	GameState(game,
			  sharedContext),
	m_returnButton(returnButton),
	m_closeButton(closeButton),
	m_blurredScene(blurredScene),
	m_FBO_A(FBO_A),
	m_FBO_B(FBO_B),
	m_sceneBlackout(blackout),
	m_blurShader(sharedContext.fileManager->getShader("Blur")),
	m_pauseText(pauseText),
	m_currentTime(currentTime),
	m_stopwatchText(stopwatch),
	m_worldShader(sharedContext.fileManager->getShader("RadialGradient"))
{
	m_returnButton->setRect(sf::FloatRect(game->getCurrentVideoMode().width - 250,
										  game->getCurrentVideoMode().height - 150,
										  100, 100));

	m_closeButton->setRect(sf::FloatRect(150, game->getCurrentVideoMode().height - 150,
										 100, 100));

	m_pauseText->setFillColor(MENU_TITLE_COLOR);

	if (*currentTime > *m_shared.bestTime)
	{
		m_shared.fileManager->saveHighScore(*currentTime);
		*m_shared.bestTime = *currentTime;
	}
}

PauseState::~PauseState()
{
}

void PauseState::clear()
{
	delete m_blurredScene;
	delete m_FBO_A;
	delete m_FBO_B;
	delete m_sceneBlackout;
	delete m_pauseText;
	delete m_returnButton;
	delete m_closeButton;
	delete m_currentTime;
	delete m_stopwatchText;
}

void PauseState::handleInput(const sf::Event & event)
{
	sf::Vector2f position;

	if (event.mouseButton.button == sf::Mouse::Left)
	{
		position = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
	}
	if (event.touch.finger == 0)
	{
		position = sf::Vector2f(event.touch.x, event.touch.y);
	}

	if (m_returnButton->isReleased(event, position))
	{
		m_game->changeState(new ResumeState(m_game,
											m_shared,
											m_currentTime,
											m_stopwatchText,
											m_blurredScene,
											m_FBO_A,
											m_FBO_B,
											m_sceneBlackout,
											m_pauseText,
											m_returnButton,
											m_closeButton,
											new PreparationState(m_game,
																 m_shared,
																 m_currentTime,
																 m_stopwatchText)));


		return;
	}
	if (m_closeButton->isReleased(event, position))
	{
		m_game->changeState(new ResumeState(m_game,
											m_shared,
											m_currentTime,
											m_stopwatchText,
											m_blurredScene,
											m_FBO_A,
											m_FBO_B,
											m_sceneBlackout,
											m_pauseText,
											m_returnButton,
											m_closeButton,
											new ToMenuTransition(m_game,
																 m_shared,
																 m_currentTime,
																 m_stopwatchText,
																 nullptr)));
		return;
	}
}

void PauseState::update(sf::Time elapsed)
{
}

void PauseState::draw(sf::RenderWindow & window)
{
	//----------------Draw scene to texture---------------------
	m_blurredScene->clear();
	m_blurredScene->setView(window.getDefaultView());
	m_blurredScene->draw(*m_shared.background);
	m_blurredScene->setView(*m_shared.gameView);

	sf::Vector2f playerPosition(m_shared.player->getPosition());
	sf::Vector2f shaderCenter(m_blurredScene->mapCoordsToPixel(playerPosition, *m_shared.gameView));
	float radius = RADIAL_SHADER_FACTOR * m_blurredScene->getSize().y / 
		m_game->getViewZoom() / PAUSE_BLUR_FACTOR / 2.f;
	m_worldShader->setUniform("radius", radius);
	m_worldShader->setUniform("center", shaderCenter);

	m_blurredScene->draw(*m_shared.world);
	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		m_blurredScene->draw(**i);
	}
	m_blurredScene->setView(window.getDefaultView());
	m_blurredScene->draw(*m_sceneBlackout);
	m_blurredScene->display();

	//---------------------Blurring scene-----------------------
	m_blurSprite.setTexture(m_blurredScene->getTexture(), true);
	m_blurSprite.setScale(1.f, 1.f);
	m_blurShader->setUniform("texture", m_blurredScene->getTexture());
	m_blurShader->setUniform("direction", sf::Glsl::Vec2(1.f, 0.f));

	m_FBO_A->clear();
	m_FBO_A->draw(m_blurSprite, m_blurShader);
	m_FBO_A->display();

	m_blurSprite.setTexture(m_FBO_A->getTexture(), true);
	m_blurSprite.setScale(1.f, 1.f);
	m_blurShader->setUniform("texture", m_FBO_A->getTexture());
	m_blurShader->setUniform("direction", sf::Glsl::Vec2(0.f, 1.f));

	m_FBO_B->clear();
	m_FBO_B->draw(m_blurSprite, m_blurShader);
	m_FBO_B->display();

	//----------Draw blurred scene and other stuff---------------
	m_blurSprite.setTexture(m_FBO_B->getTexture(), true);
	m_blurSprite.setScale(1.f / PAUSE_BLUR_FACTOR, 1.f / PAUSE_BLUR_FACTOR);

	window.setView(window.getDefaultView());
	window.draw(m_blurSprite);
	window.setView(*m_shared.gameView);
	window.draw(*m_shared.player);

	window.setView(window.getDefaultView());
	window.draw(*m_returnButton);
	window.draw(*m_closeButton);
	window.draw(*m_stopwatchText);
	window.draw(*m_pauseText);
}

