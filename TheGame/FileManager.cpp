#include "FileManager.h"



FileManager::FileManager()
{
}


FileManager::~FileManager()
{
	for (auto i = m_fonts.begin(); i != m_fonts.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_fonts.clear();

	for (auto i = m_soundBuffers.begin(); i != m_soundBuffers.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_soundBuffers.clear();

	for (auto i = m_shaders.begin(); i != m_shaders.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	m_shaders.clear();

	delete m_spawnPresets;
	m_spawnPresets = nullptr;
}

void FileManager::loadFont(const std::string & fontName, const std::string & src)
{
	sf::Font* font = new sf::Font;
	font->loadFromFile(src);

	addFont(fontName, font);
}

void FileManager::loadSound(const std::string & soundName, const std::string & src)
{
	sf::SoundBuffer* buffer = new sf::SoundBuffer;
	buffer->loadFromFile(src);

	addSound(soundName, buffer);
}

void FileManager::loadShader(const std::string & shaderName, const std::string & fragment, const std::string & vertex)
{
	sf::Shader* shader = new sf::Shader;
	shader->loadFromFile(vertex, fragment);

	addShader(shaderName, shader);

}

void FileManager::loadSpawnPresets(const std::string & src)
{
	m_spawnPresets = new SpawnPresets(src);
}

void FileManager::addFont(const std::string & fontName, sf::Font * font)
{
	m_fonts.emplace(fontName, font);
}

void FileManager::addSound(const std::string & soundName, sf::SoundBuffer * sound)
{
	m_soundBuffers.emplace(soundName, sound);
}

void FileManager::addShader(const std::string & shaderName, sf::Shader * shader)
{
	m_shaders.emplace(shaderName, shader);
}

sf::Font * FileManager::getFont(const std::string & fontName) const
{
	return m_fonts.at(fontName);
}

sf::SoundBuffer * FileManager::getSound(const std::string & soundName) const
{
	return m_soundBuffers.at(soundName);
}

SpawnPresets * FileManager::getSpawnPresets() const
{
	return m_spawnPresets;
}

sf::Shader * FileManager::getShader(const std::string & shaderName) const
{
	return m_shaders.at(shaderName);
}
