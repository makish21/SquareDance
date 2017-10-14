#include "MainMenuState.h"
#include "Definitions.hpp"

MainMenuState::MainMenuState(Game* game) :
	GameState(game),
	m_playButton(game->getPlayer()->getRect())
{
	m_sound.setBuffer(*game->getFileManager()->getSound("Space"));
	m_sound.setLoop(true);
	m_sound.play();

	m_playButton.setPosition(sf::Vector2f(game->getPlayer()->getRect().left,
										  game->getPlayer()->getRect().top));

	game->setViewZoom(MENU_VIEW_ZOOM);
	game->getPlayer()->setPosition(MENU_PLAYER_POSITION);
	game->getWorld()->setBoundsColor(MENU_WORLD_COLOR);
	game->setTitleColor(MENU_TITLE_COLOR);
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::draw(sf::RenderWindow& window)
{
}

void MainMenuState::update(sf::Time elapsed)
{
	m_game->getPlayer()->rotate(-10.f * elapsed.asSeconds());

	m_playButton.setSize(sf::Vector2f(m_game->getPlayer()->getRect().width,
									  m_game->getPlayer()->getRect().height));
	m_playButton.setPosition(sf::Vector2f(m_game->getPlayer()->getRect().left,
										  m_game->getPlayer()->getRect().top));
}

void MainMenuState::handleInput(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			m_game->close();
			return;
		}
	}

	if (m_playButton.isReleased(event, m_game->getWorldMousePosition(*m_game->getView())))
	{
		m_game->changeState(new ToGameTransition(m_game));
		return;
	}
}