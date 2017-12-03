#include "ToPauseTransition.h"
#include "PauseState.h"
#include "Animations.h"
#include "Definitions.hpp"

ToPauseTransition::ToPauseTransition(Game* const game,
									 const SharedContext& sharedContext,
									 sf::Time* const currentTime,
									 sf::Text* const stopwatch) :
	GameState(game,
			  sharedContext),
	m_returnButton(new RenderButton(sf::FloatRect(game->getCurrentVideoMode().width - 250,
												  game->getCurrentVideoMode().height,
												  100, 100))),
	m_closeButton(new RenderButton(sf::FloatRect(150, game->getCurrentVideoMode().height,
												 100, 100))),
	m_sceneBlackout(new sf::RectangleShape(sf::Vector2f(game->getCurrentVideoMode().width,
														game->getCurrentVideoMode().height))),
	m_blurShader(sharedContext.fileManager->getShader("Blur")),
	m_pauseText(new sf::Text("Pause", *sharedContext.fileManager->getFont("Helvetica"))),
	m_blurredScene(new sf::RenderTexture),
	m_FBO_A(new sf::RenderTexture),
	m_FBO_B(new sf::RenderTexture),
	m_currentTime(currentTime),
	m_stopwatchText(stopwatch)
{
	m_returnButton->setTexture(*sharedContext.fileManager->getTexture("ResumeIcon"));
	m_returnButton->setAlignment(Center, Center);
	m_returnButton->setFitting(Fit, 0.5f);

	m_closeButton->setTexture(*sharedContext.fileManager->getTexture("ExitIcon"));
	m_closeButton->setAlignment(Center, Center);
	m_closeButton->setFitting(Fit, 0.4f);

	m_blurShader->loadFromFile("Blur.vert", "Blur.frag");
	m_blurShader->setUniform("resolution", sf::Glsl::Vec2(game->getCurrentVideoMode().width,
														  game->getCurrentVideoMode().height) * PAUSE_BLUR_FACTOR);

	m_blurredScene->create(game->getCurrentVideoMode().width * PAUSE_BLUR_FACTOR,
						  game->getCurrentVideoMode().height * PAUSE_BLUR_FACTOR);
	m_blurredScene->setSmooth(true);

	m_FBO_A->create(game->getCurrentVideoMode().width * PAUSE_BLUR_FACTOR,
				   game->getCurrentVideoMode().height * PAUSE_BLUR_FACTOR);
	m_FBO_A->setSmooth(true);

	m_FBO_B->create(game->getCurrentVideoMode().width * PAUSE_BLUR_FACTOR,
				   game->getCurrentVideoMode().height * PAUSE_BLUR_FACTOR);
	m_FBO_B->setSmooth(true);

	m_sceneBlackout->setFillColor(sf::Color::Transparent);
	m_pauseText->setCharacterSize(static_cast<unsigned int>(CHARACTER_SIZE_FACTOR *
															TITLE_CHARACTER_SIZE *
															game->getBestVideoMode().height));
	m_pauseText->setScale(float(game->getCurrentVideoMode().width) / float(game->getBestVideoMode().width),
						  float(game->getCurrentVideoMode().width) / float(game->getBestVideoMode().width));
	m_pauseText->setFillColor(MENU_TITLE_COLOR);
	sf::FloatRect textRect = m_pauseText->getLocalBounds();
	m_pauseText->setOrigin(textRect.left + textRect.width / 2,
						   textRect.top + textRect.height / 2);
	m_pauseText->setPosition(static_cast<float>(game->getCurrentVideoMode().width) / 2,
							 TITLE_Y_POSITION_FACTOR *
							 static_cast<float>(game->getCurrentVideoMode().height));

	sf::FloatRect playerRect(sf::Vector2f(game->mapCoordsToPixel(sf::Vector2f(sharedContext.player->getRect().left,
																			  sharedContext.player->getRect().top),
																 *sharedContext.gameView)),
							 sf::Vector2f(game->mapCoordsToPixel(sf::Vector2f(sharedContext.player->getRect().width,
																			  sharedContext.player->getRect().height),
																 *sharedContext.gameView)));

	if (m_pauseText->getGlobalBounds().intersects(playerRect))
	{
		m_pauseText->setPosition(static_cast<float>(game->getCurrentVideoMode().width) / 2,
								 (1.f - TITLE_Y_POSITION_FACTOR) *
								 static_cast<float>(game->getCurrentVideoMode().height));
	}
}


ToPauseTransition::~ToPauseTransition()
{
}

void ToPauseTransition::clear()
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

void ToPauseTransition::handleInput(const sf::Event & event)
{
}

void ToPauseTransition::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	float buttonsTop;
	sf::FloatRect rect;

	if (m_elapsedTime.asSeconds() < TO_PAUSE_TRANSITION_DURATION)
	{
		// Pause appearing
		m_pauseText->setFillColor(transfer(m_elapsedTime,
										   sf::seconds(TO_PAUSE_TRANSITION_DURATION),
										   INITIAL_TITLE_COLOR,
										   MENU_TITLE_COLOR));

	    // Blur appearing
		m_blurColor = transfer(m_elapsedTime,
							   sf::seconds(TO_PAUSE_TRANSITION_DURATION),
							   sf::Color(255, 255, 255, 0),
							   sf::Color(255, 255, 255, 255));

		// Blackout filter
		m_sceneBlackout->setFillColor(transfer(m_elapsedTime,
											   sf::seconds(TO_PAUSE_TRANSITION_DURATION),
											   sf::Color(0, 0, 0, 0),
											   sf::Color(0, 0, 0, 80)));

		 // Change buttons Y-position
		buttonsTop = transfer(m_elapsedTime, sf::seconds(TO_PAUSE_TRANSITION_DURATION),
							  m_game->getCurrentVideoMode().height,
							  m_game->getCurrentVideoMode().height - 150.f);

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
		m_game->changeState(new PauseState(m_game,
										   m_shared,
										   m_currentTime,
										   m_stopwatchText,
										   m_blurredScene,
										   m_FBO_A,
										   m_FBO_B,
										   m_sceneBlackout,
										   m_pauseText,
										   m_returnButton,
										   m_closeButton));

		return;
	}
}

void ToPauseTransition::draw(sf::RenderWindow & window)
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

