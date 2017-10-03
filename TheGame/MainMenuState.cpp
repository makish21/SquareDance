#include "MainMenuState.h"


MainMenuState::MainMenuState(Game* game) :
	GameState(game),
	m_playButton(game->getPlayer()->getRect())
{
	m_sound.setBuffer(*game->getSoundBuffer("Space"));
	m_sound.setLoop(true);
	m_sound.play();

	m_playButton.setPosition(sf::Vector2f(game->getPlayer()->getRect().left, game->getPlayer()->getRect().top));

	game->setViewZoom(VIEW_ZOOM);
	game->getPlayer()->setPosition(PLAYER_POSITION);
	game->getWorld()->setAlpha(WORLD_TRANSPARENCY);
	game->setTitleAlpha(255);
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::draw(sf::RenderWindow& window)
{
}

void MainMenuState::update(float time)
{
	m_game->getPlayer()->rotate(-0.2f);

	if (m_playButton.isReleased())
	{
		m_game->changeState(new ToGameTransition(m_game));
		return;
	}

	m_playButton.setSize(sf::Vector2f(m_game->getPlayer()->getRect().width,
									  m_game->getPlayer()->getRect().height));
	m_playButton.setPosition(sf::Vector2f(m_game->getPlayer()->getRect().left,
										  m_game->getPlayer()->getRect().top));

	m_playButton.update();
}

void MainMenuState::handleInput(sf::Event& event)
{
	while (m_game->getWindow()->pollEvent(event))
	{
		m_game->handleEvent(event);

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_game->close();
				return;
			}
		}
	}

	m_playButton.handleInput(event, m_game->getWorldMousePosition(*m_game->getView()));
}