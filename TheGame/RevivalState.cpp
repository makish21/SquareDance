#include "RevivalState.h"
#include "Definitions.hpp"
#include "PreparationState.h"
#include "ToMenuTransition.h"
#include "CrossPlatform.hpp"

RevivalState::RevivalState(Game* const game,
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
	m_particleSystem(99, game->getWindow()->getSize().x),
	m_elapsed(sf::milliseconds(0)),
	m_deathSound(*m_fileManager->getSound("RecordRewind"))
{
	m_particleSystem.setEmitter(player->getPosition());
	m_particleSystem.setDirection(player->getVelocity());
	m_particleSystem.resetParticles();

	m_deathSound.play();
#if defined(__ANDROID__)
	vibrate(sf::milliseconds(100));
#endif

	player->setScale(0.f, 0.f);
	player->setPosition(400.f, 140.f);
	player->setRotation(45.f);
}

RevivalState::~RevivalState()
{
}

void RevivalState::handleInput(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			m_game->changeState(new ToMenuTransition(m_game,
													 m_fileManager,
													 m_gameView,
													 m_player,
													 m_enemySpawner,
													 m_objects,
													 m_world));
			return;
		}
	}
}

void RevivalState::update(sf::Time elapsed)
{
	m_elapsed += elapsed;
	float elapsedTime = m_elapsed.asSeconds();

	if (elapsedTime < PLAYER_REVIVAL_DURATION)
	{
		float timeFactor = std::sin(elapsedTime / PLAYER_REVIVAL_DURATION * PI / 2.f);
		float speedFactor = timeFactor * 4.f + 1.f;

		for (auto i = m_objects->begin(); i != m_objects->end(); i++)
		{
			(*i)->update(*m_world, elapsed * speedFactor);
		}
		m_particleSystem.update(elapsed);

		m_player->setScale(timeFactor, timeFactor);
	}
	else
	{
		m_game->changeState(new PreparationState(m_game,
												 m_fileManager,
												 m_gameView,
												 m_player,
												 m_enemySpawner,
												 m_objects,
												 m_world));
		return;
	}
}

void RevivalState::draw(sf::RenderWindow & window)
{
	window.draw(m_particleSystem);
}
