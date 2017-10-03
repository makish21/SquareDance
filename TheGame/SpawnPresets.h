#pragma once
#include <deque>
#include <vector>
#include <fstream>
#include <sstream>

#include "Enemy.h"
#include "DefaultEnemy.h"

struct SpawnGroup
{
	std::deque<Enemy*> m_enemies;
	float m_breakTime;
};

using SpawnPreset = std::deque<SpawnGroup>;

class SpawnPresets
{
public:
	SpawnPresets();
	SpawnPresets(const std::string& file);
	~SpawnPresets();

	void loadFromFile(const std::string& file);
	size_t getSize() const;

	const SpawnPreset& operator[](size_t index) const;

private:
	std::vector<SpawnPreset> m_presets;
};