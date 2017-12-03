#include "ResumeState.h"
#include "Animations.h"


ResumeState::ResumeState(Game * game,
						 const SharedContext & sharedContext,
						 sf::Time* const currentTime,
						 sf::Text* const stopwatch,
						 sf::RenderTexture*  blurredScene,
						 sf::RenderTexture*  FBO_A,
						 sf::RenderTexture*  FBO_B,
						 sf::RectangleShape* blackout,
						 sf::Text*     pauseText,
						 RenderButton* returnButton,
						 RenderButton* closeButton,
						 GameState* nextState) :
	GameState(game, sharedContext),
	m_blurShader(sharedContext.fileManager->getShader("Blur")),
	m_blurredScene(blurredScene),
	m_FBO_A(FBO_A),
	m_FBO_B(FBO_B),
	m_sceneBlackout(blackout),
	m_pauseText(pauseText),
	m_returnButton(returnButton),
	m_closeButton(closeButton),
	m_nextState(nextState),
	m_currentTime(currentTime),
	m_stopwatchText(stopwatch)
{
}

ResumeState::~ResumeState()
{
	delete m_blurredScene;
	delete m_FBO_A;
	delete m_FBO_B;
	delete m_sceneBlackout;
	delete m_returnButton;
	delete m_closeButton;
}

void ResumeState::clear()
{
	delete m_currentTime;
	delete m_stopwatchText;
}

void ResumeState::handleInput(const sf::Event & event)
{
}

void ResumeState::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	float buttonsTop;
	sf::FloatRect rect;

	if (m_elapsedTime.asSeconds() < RESUME_DURATION)
	{
		// Pause appearing
		m_pauseText->setFillColor(transfer(m_elapsedTime,
										   sf::seconds(RESUME_DURATION),
										   MENU_TITLE_COLOR,
										   INITIAL_TITLE_COLOR));

		// Blur appearing
		m_blurColor = transfer(m_elapsedTime,
							   sf::seconds(RESUME_DURATION),
							   sf::Color(255, 255, 255, 255),
							   sf::Color(255, 255, 255, 0));

		// Blackout filter
		m_sceneBlackout->setFillColor(transfer(m_elapsedTime,
											   sf::seconds(RESUME_DURATION),
											   sf::Color(0, 0, 0, 80),
											   sf::Color(0, 0, 0, 0)));

		// Change buttons Y-position
		buttonsTop = transfer(m_elapsedTime, sf::seconds(RESUME_DURATION),
							  m_game->getCurrentVideoMode().height - 150.f,
							  m_game->getCurrentVideoMode().height);

		// Change return button position
		rect = m_returnButton->getRect();
		rect.top = buttonsTop;
		m_returnButton->setRect(rect);

		// Change close button position
		rect = m_closeButton->getRect();
		rect.top = buttonsTop;
		m_closeButton->setRect(rect);
	}
	else
	{
		m_game->changeState(m_nextState);
		return;
	}
}

void ResumeState::draw(sf::RenderWindow & window)
{
	//----------------Draw scene to texture---------------------
	m_blurredScene->clear();
	m_blurredScene->setView(window.getDefaultView());
	m_blurredScene->draw(*m_shared.background);
	m_blurredScene->setView(*m_shared.gameView);
	m_blurredScene->draw(*m_shared.world);
	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		m_blurredScene->draw(**i);
	}
	m_blurredScene->setView(window.getDefaultView());
	m_blurredScene->draw(*m_sceneBlackout);
	m_blurredScene->display();

	//---------------------Blurring scene-----------------------
	m_blurSprite.setTexture(m_blurredScene->getTexture());
	m_blurShader->setUniform("texture", m_blurredScene->getTexture());
	m_blurShader->setUniform("direction", sf::Glsl::Vec2(1.f, 0.f));

	m_FBO_A->clear();
	m_FBO_A->draw(m_blurSprite, m_blurShader);
	m_FBO_A->display();

	m_blurSprite.setTexture(m_FBO_A->getTexture());
	m_blurShader->setUniform("texture", m_FBO_A->getTexture());
	m_blurShader->setUniform("direction", sf::Glsl::Vec2(0.f, 1.f));

	m_FBO_B->clear();
	m_FBO_B->draw(m_blurSprite, m_blurShader);
	m_FBO_B->display();

	//-----------------Draw blurred scene and other stuff---------------------
	m_blurSprite.setTexture(m_FBO_B->getTexture());
	m_blurSprite.setScale(1.f / PAUSE_BLUR_FACTOR, 1.f / PAUSE_BLUR_FACTOR);
	m_blurSprite.setColor(m_blurColor);

	window.setView(window.getDefaultView());
	window.draw(*m_shared.background);
	window.setView(*m_shared.gameView);
	window.draw(*m_shared.world);

	for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		window.draw(**i);
	}

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
