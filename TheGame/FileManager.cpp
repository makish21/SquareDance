#include "FileManager.h"
#include "CrossPlatform.hpp"

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

	for (auto i = m_textures.begin(); i != m_textures.end(); i++)
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

void FileManager::loadTexture(const std::string & textureName, const std::string & src)
{
	sf::Texture* texture = new sf::Texture;
	texture->loadFromFile(src);
	texture->setSmooth(true);
	texture->setRepeated(true);

	addTexture(textureName, texture);
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
	m_fontsMutex.lock();

	m_fonts.emplace(fontName, font);

	m_fontsMutex.unlock();
}

void FileManager::addTexture(const std::string & textureName, sf::Texture * texture)
{
	m_texturesMutex.lock();

	m_textures.emplace(textureName, texture);

	m_texturesMutex.unlock();
}

void FileManager::addSound(const std::string & soundName, sf::SoundBuffer * sound)
{
	m_soundsMutex.lock();

	m_soundBuffers.emplace(soundName, sound);

	m_soundsMutex.unlock();
}

void FileManager::addShader(const std::string & shaderName, sf::Shader * shader)
{
	m_shadersMutex.lock();

	m_shaders.emplace(shaderName, shader);

	m_shadersMutex.unlock();
}

sf::Font * FileManager::getFont(const std::string & fontName) const
{
	auto it = m_fonts.find(fontName);

	return it != m_fonts.end() ? it->second : nullptr;
}

sf::Texture * FileManager::getTexture(const std::string & textureName) const
{
	auto it = m_textures.find(textureName);

	return it != m_textures.end() ? it->second : nullptr;
}

sf::SoundBuffer * FileManager::getSound(const std::string & soundName) const
{
	auto it = m_soundBuffers.find(soundName);

	return it != m_soundBuffers.end() ? it->second : nullptr;
}

sf::Shader * FileManager::getShader(const std::string & shaderName) const
{
	auto it = m_shaders.find(shaderName);

	return it != m_shaders.end() ? it->second : nullptr;
}

SpawnPresets * FileManager::getSpawnPresets() const
{
	return m_spawnPresets;
}

sf::Time FileManager::loadHighScore() const
{
	sf::Time time;
	sys::loadHighScore(time);
	return time;
}

void FileManager::saveHighScore(sf::Time score) const
{
	std::thread thread(sys::saveHighScore, score);
	thread.detach();
	//sys::saveHighScore(score);
}
