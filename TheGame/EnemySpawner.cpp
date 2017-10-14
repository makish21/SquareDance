#include "EnemySpawner.h"
#include "Definitions.hpp"


EnemySpawner::EnemySpawner(std::list<Object*>* container,
						   SpawnPresets* presets) :
	m_container(container),
	m_spawnPresets(presets),
	m_distribution(0, presets->getSize() - 1),
	m_randomGenerator(m_randomDevice()),
	m_breakTime(FIRST_SPAWN_TIME_DELAY),
	m_elapsed(0.f)
{
	reset();
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::reset()
{
	while (!m_spawnGroups.empty())
	{
		m_spawnGroups.pop();
	}

	fillQueue();
}

void EnemySpawner::update(sf::Time elapsed, float speed)
{
	m_elapsed += elapsed.asSeconds() * speed;//m_clock.getElapsedTime().asSeconds();

	if (m_elapsed >= m_breakTime)
	{
		m_breakTime -= m_elapsed;

		for (auto i = m_spawnGroups.front().m_enemies.begin();
			 i != m_spawnGroups.front().m_enemies.end();
			 i++)	
		{
			m_container->push_back((*i)->clone());
		}

		m_breakTime += m_spawnGroups.front().m_breakTime;

		m_spawnGroups.pop();
		m_elapsed = 0.f;
		//m_clock.restart();
	}

	fillQueue();
}

void EnemySpawner::setSpawnPresets(const SpawnPresets * presets)
{
	m_spawnPresets = presets;
	m_distribution = std::uniform_int_distribution<size_t>(0, presets->getSize() - 1);
}

void EnemySpawner::fillQueue()
{
	while (m_spawnGroups.size() < MAX_SPAWNER_QUEUE_SIZE)
	{
		addPreset();
	}
}

void EnemySpawner::addPreset()
{
	size_t key = m_distribution(m_randomGenerator);

	for (auto i = (*m_spawnPresets)[key].begin();
		 i != (*m_spawnPresets)[key].end();
		 i++)
	{
		m_spawnGroups.push(*i);
	}
}
