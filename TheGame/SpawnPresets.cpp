#include "SpawnPresets.h"
#include "tinyxml.h"
#include "Definitions.hpp"

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

void SpawnPresets::loadFromFile(const std::string & fileName)
{
	sf::FileInputStream file;
	file.open(fileName);

	char* content = new char[file.getSize() + 1];
	file.read(content, file.getSize());
	content[file.getSize()] = '\0';

	TiXmlDocument doc;
	doc.Parse(content, 0, TIXML_ENCODING_UTF8);
	TiXmlElement* root = doc.FirstChildElement();

	//TiXmlElement* pSpawnersNode = hRoot.FirstChild("preset").Element();

	for (TiXmlElement* p_preset = root->FirstChildElement(); p_preset; p_preset = p_preset->NextSiblingElement())
	{
		SpawnPreset preset;

		for (TiXmlElement* p_group = p_preset->FirstChildElement(); p_group; p_group = p_group->NextSiblingElement())
		{
			SpawnGroup group;
			p_group->QueryFloatAttribute("break-time", &group.m_breakTime);

			for (TiXmlElement* p_enemy = p_group->FirstChildElement(); p_enemy; p_enemy = p_enemy->NextSiblingElement())
			{
				std::string enemyName = p_enemy->Value();

				if (enemyName == "DefaultEnemy")
				{
					float y, dir;

					if (std::string(p_enemy->Attribute("y")) == "random")
					{
						y = static_cast<float>(std::rand() % 1000) / 1000.f;
					}
					else
					{
						p_enemy->QueryFloatAttribute("y", &y);
					}

					y = y * (WORLD_SIZE.y - 30.f) + 15.f;
					p_enemy->QueryFloatAttribute("dir", &dir);

					group.m_enemies.push_back(new DefaultEnemy(y, dir));
				}
			}

			preset.push_back(group);
		}

		m_presets.push_back(preset);
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
