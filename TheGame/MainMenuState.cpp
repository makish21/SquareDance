#include "MainMenuState.h"
#include "Definitions.hpp"

MainMenuState::MainMenuState(Game* const game,
							 const SharedContext& sharedContext) :
	GameState(game,
			  sharedContext)
{
	game->setViewZoom(MENU_VIEW_ZOOM);
	sharedContext.player->setScale(1.f, 1.f);
	sharedContext.player->setPosition(MENU_PLAYER_POSITION);
	sharedContext.world->setBoundsColor(MENU_WORLD_COLOR);
	game->setTitleColor(MENU_TITLE_COLOR);

	sf::FloatRect buttonRect;
	buttonRect.left = sharedContext.player->getPosition().x - PLAYER_RADIUS;
	buttonRect.top = sharedContext.player->getPosition().y - PLAYER_RADIUS;
	buttonRect.width = PLAYER_RADIUS * 2.f;
	buttonRect.height = PLAYER_RADIUS * 2.f;
	m_playButton.setRect(buttonRect);
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::draw(sf::RenderWindow& window)
{
	window.draw(*m_shared.background);

	window.setView(*m_shared.gameView);

	//window.draw(*m_shared.world);
	window.draw(*m_shared.player);

	/*for (auto i = m_shared.objects->begin(); i != m_shared.objects->end(); i++)
	{
		window.draw(**i);
	}*/
}

void MainMenuState::update(sf::Time elapsed)
{
	m_shared.player->rotate(-10.f * elapsed.asSeconds());
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

	sf::Vector2i position;

	if (event.mouseButton.button == sf::Mouse::Left)
	{
		position = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
	}
	if (event.touch.finger == 0)
	{
		position = sf::Vector2i(event.touch.x, event.touch.y);
	}

	if (m_playButton.isReleased(event, m_game->mapPixelToCoords(position, *m_shared.gameView)))
	{
		m_game->changeState(new ToGameTransition(m_game,
												 m_shared));
		return;
	}
}