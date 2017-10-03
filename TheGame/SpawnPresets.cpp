#include "SpawnPresets.h"

SpawnPresets::SpawnPresets()
{
}

SpawnPresets::SpawnPresets(const std::string & file)
{
	loadFromFile(file);
}

SpawnPresets::~SpawnPresets()
{
	for (auto i = m_presets.begin(); i != m_presets.end(); i++)
	{
		for (auto j = i->begin(); j != i->end(); j++)
		{
			for (auto k = j->m_enemies.begin(); k != j->m_enemies.end(); k++)
			{
				if (*k)
				{
					delete *k;
					*k = nullptr;
				}
			}
			j->m_enemies.clear();
		}
		i->clear();
	}
	m_presets.clear();
}

void SpawnPresets::loadFromFile(const std::string & file)
{
	sf::FileInputStream presets;
	presets.open(file);

	char* content = new char[presets.getSize() + 1];
	presets.read(content, presets.getSize());
	content[presets.getSize()] = '\0';

	std::istringstream stream(content);
	std::string line;

	while (std::getline(stream, line))
	{
		std::string type;
		bool preset = false;

		std::stringstream keyStream(line);
		keyStream >> type;

		if (type == "PRESET_BEGIN")
		{
			preset = true;
		}

		SpawnPreset spawnPreset;

		while (preset)
		{
			bool group = false;

			keyStream >> type;

			if (type == "GROUP_BEGIN")
			{
				group = true;
			}
			if (type == "PRESET_END")
			{
				m_presets.push_back(spawnPreset);
				preset = false;
			}

			SpawnGroup spawnGroup;

			while (group)
			{
				keyStream >> type;

				if (type == "DEFAULT_ENEMY")
				{
					float y, velocityX;
					keyStream >> y >> velocityX;

					spawnGroup.m_enemies.push_back(new DefaultEnemy(y, velocityX));
				}

				if (type == "BREAK_TIME")
				{
					keyStream >> spawnGroup.m_breakTime;
				}

				if (type == "GROUP_END")
				{
					spawnPreset.push_back(spawnGroup);
					group = false;
				}
			}
		}
	}
	delete[] content;
}

size_t SpawnPresets::getSize() const
{
	return m_presets.size();
}

const SpawnPreset & SpawnPresets::operator[](size_t index) const
{
	return m_presets[index];
}
