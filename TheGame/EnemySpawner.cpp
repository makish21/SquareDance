#include "EnemySpawner.h"


EnemySpawner::EnemySpawner(std::list<Object*>* container) :
	m_container(container),
	m_breakTime(4.f)
{
	m_clock.restart();
	m_randomGenerator.seed(m_randomDevice());
}

EnemySpawner::EnemySpawner(std::list<Object*>* container,
						   SpawnPresets* presets) :
	m_container(container),
	m_spawnPresets(presets),
	m_distribution(0, presets->getSize() - 1),
	m_randomGenerator(m_randomDevice()),
	m_breakTime(2.f)
{
	m_clock.restart();
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::update(float speed)
{
	float elapsed = m_clock.getElapsedTime().asSeconds();

	if (elapsed * speed >= m_breakTime)
	{
		m_breakTime -= elapsed * speed;

		for (auto i = m_spawnGroups.front().m_enemies.begin();
			 i != m_spawnGroups.front().m_enemies.end();
			 i++)	
		{
			m_container->push_back((*i)->clone());
		}

		m_breakTime += m_spawnGroups.front().m_breakTime;

		m_spawnGroups.pop();
		m_clock.restart();
	}

	while (m_spawnGroups.size() < 5)
	{
		addGroups();
	}
}

void EnemySpawner::setSpawnPresets(const SpawnPresets * presets)
{
	m_spawnPresets = presets;
	m_distribution = std::uniform_int_distribution<size_t>(0, presets->getSize() - 1);
}

void EnemySpawner::addGroups()
{
	size_t key = m_distribution(m_randomGenerator);

	for (auto i = (*m_spawnPresets)[key].begin();
		 i != (*m_spawnPresets)[key].end();
		 i++)
	{
		m_spawnGroups.push(*i);
	}
}
