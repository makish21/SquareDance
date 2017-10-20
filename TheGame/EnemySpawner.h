#pragma once
//#include <list>
#include <queue>
#include <random>
#include <SFML\Graphics.hpp>

#include "Definitions.hpp"
#include "SpawnPresets.h"

class EnemySpawner
{
public:
	EnemySpawner(std::list<Object*>* container);
	EnemySpawner(std::list<Object*>* container, SpawnPresets* presets);
	~EnemySpawner();

	void reset();
	void update(sf::Time elapsed, float speed);
	void setSpawnPresets(const SpawnPresets* presets);

private:
	void fillQueue();
	void addPreset();

	//sf::Clock m_clock;
	float m_elapsed;
	float m_breakTime;

	std::random_device m_randomDevice;
	std::mt19937 m_randomGenerator;
	std::uniform_int_distribution<size_t> m_distribution;

	std::list<Object*>* m_container;
	std::queue<SpawnGroup> m_spawnGroups;
	const SpawnPresets* m_spawnPresets;
};