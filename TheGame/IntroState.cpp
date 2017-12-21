#include "IntroState.h"
#include "ToMenuTransition.h"
#include "Animations.h"

IntroState::IntroState(Game * const game,
					   const SharedContext& sharedContext) :
	GameState(game,
			  sharedContext),
	m_loadDataThread(&IntroState::loadData, this),
	m_textColor(sf::Color::Black),
	m_introDuration(sf::seconds(MIN_INTRO_DURATION)),
	m_dataLoaded(false)
{
	float characterSize = CHARACTER_SIZE_FACTOR * game->getBestVideoMode().height;
	sf::FloatRect textRect;

	m_developerText.setFont(*m_shared.fileManager->getFont("Text"));
	m_developerNameText.setFont(*m_shared.fileManager->getFont("Titles"));
	m_createdWithText.setFont(*m_shared.fileManager->getFont("Text"));

	m_developerText.setCharacterSize(static_cast<unsigned int>(characterSize * 25));
	m_developerNameText.setCharacterSize(static_cast<unsigned int>(characterSize * 45));
	m_createdWithText.setCharacterSize(static_cast<unsigned int>(characterSize * 15));

	m_developerText.setString("Developer:");
	m_developerNameText.setString("Maxim Ishenkin");
	m_createdWithText.setString("Created with SFML");

	m_developerText.setFillColor(m_textColor);
	m_developerNameText.setFillColor(m_textColor);
	m_createdWithText.setFillColor(m_textColor);

	textRect = m_developerText.getLocalBounds();
	m_developerText.setOrigin(textRect.left + textRect.width / 2.f,
							  textRect.top + textRect.height / 2.f + 40.f);
	textRect = m_developerNameText.getLocalBounds();
	m_developerNameText.setOrigin(textRect.left + textRect.width / 2.f,
								  textRect.top + textRect.height / 2.f - 10.f);

	textRect = m_createdWithText.getLocalBounds();

	m_createdWithText.setOrigin(textRect.left + textRect.width / 2.f,
								textRect.top + textRect.height);

	m_developerText.setPosition(game->getCurrentVideoMode().width / 2,
								game->getCurrentVideoMode().height / 2);
	m_developerNameText.setPosition(game->getCurrentVideoMode().width / 2,
									game->getCurrentVideoMode().height / 2);
	m_createdWithText.setPosition(game->getCurrentVideoMode().width / 2,
								  game->getCurrentVideoMode().height - 10);

	m_loadDataThread.detach();
}

IntroState::~IntroState()
{
}

void IntroState::clear()
{
}

void IntroState::handleInput(const sf::Event & event)
{
}

void IntroState::update(sf::Time elapsed)
{
	m_elapsedTime += elapsed;

	if (m_elapsedTime > m_introDuration)
	{
		if (m_dataLoaded)
		{
			m_textColor = transfer(m_elapsedTime - m_introDuration,
								   sf::seconds(1.f), sf::Color::Black,
								   sf::Color(0, 0, 0, 0));

			m_developerText.setFillColor(m_textColor);
			m_developerNameText.setFillColor(m_textColor);
			m_createdWithText.setFillColor(m_textColor);
		}
		else
		{
			m_introDuration += sf::seconds(1.f);
		}
	}
	if (m_elapsedTime > m_introDuration + sf::seconds(1.f))
	{
		m_game->changeState(new ToMenuTransition(m_game,
												 m_shared,
												 nullptr,
												 nullptr,
												 nullptr));
	}
}

void IntroState::draw(sf::RenderWindow & window)
{
	window.setView(*m_shared.gameView);

	window.draw(*m_shared.player);

	window.setView(window.getDefaultView());

	window.draw(m_developerText);
	window.draw(m_developerNameText);
	window.draw(m_createdWithText);
}

void IntroState::loadData()
{
	m_shared.fileManager->loadSound("RecordRewind", "Music/RecordRewind.ogg");

	m_shared.fileManager->loadTexture("PauseIcon", "Textures/pause.png");
	m_shared.fileManager->loadTexture("ResumeIcon", "Textures/return.png");
	m_shared.fileManager->loadTexture("ExitIcon", "Textures/close.png");

	m_shared.fileManager->loadShader("Blur", "Shaders/Blur.frag", "Shaders/Blur.vert");

	*m_shared.bestTime = m_shared.fileManager->loadHighScore();

	m_dataLoaded = true;
}
