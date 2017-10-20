#include "MainMenuState.h"
#include "Definitions.hpp"

MainMenuState::MainMenuState(Game* const game,
							 FileManager* const fileManager,
							 sf::View* const view,
							 Player* const player,
							 EnemySpawner* const enemySpawner,
							 GameObjects* const gameObjects,
							 World* const world) :
	GameState(game,
			  fileManager,
			  view,
			  player,
			  enemySpawner,
			  gameObjects,
			  world),
	m_playButton(player->getRect())
{
	m_playButton.setPosition(sf::Vector2f(player->getRect().left,
										  player->getRect().top));

	game->setViewZoom(MENU_VIEW_ZOOM);
	player->setPosition(MENU_PLAYER_POSITION);
	world->setBoundsColor(MENU_WORLD_COLOR);
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
	m_player->rotate(-10.f * elapsed.asSeconds());

	m_playButton.setSize(sf::Vector2f(m_player->getRect().width,
									  m_player->getRect().height));
	m_playButton.setPosition(sf::Vector2f(m_player->getRect().left,
										  m_player->getRect().top));
}

void MainMenuState::handleInput(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			m_game->close();
			return;
		}
	}

	if (m_playButton.isReleased(event, m_game->getWorldMousePosition(*m_gameView)))
	{
		m_game->changeState(new ToGameTransition(m_game,
												 m_fileManager,
												 m_gameView,
												 m_player,
												 m_enemySpawner,
												 m_objects,
												 m_world));
		return;
	}
}