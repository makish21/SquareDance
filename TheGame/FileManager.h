#pragma once
#include <unordered_map>
#include <thread>
#include <mutex>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

#include "EnemySpawner.h"

class FileManager : sf::NonCopyable
{
public:
	FileManager();
	~FileManager();

	void loadFont(const std::string& fontName, const std::string& src);
	void loadTexture(const std::string& textureName, const std::string& src);
	void loadSound(const std::string& soundName, const std::string& src);
	void loadShader(const std::string& shaderName, const std::string& fragment, const std::string& vertex);
	void loadSpawnPresets(const std::string& src);

	void addFont(const std::string& fontName, sf::Font* font);
	void addTexture(const std::string& textureName, sf::Texture* texture);
	void addSound(const std::string& soundName, sf::SoundBuffer* sound);
	void addShader(const std::string& shaderName, sf::Shader* shader);
	
	sf::Font*        getFont(const std::string& fontName) const;
	sf::Texture*     getTexture(const std::string& textureName) const;
	sf::SoundBuffer* getSound(const std::string& soundName) const;
	sf::Shader*      getShader(const std::string& shaderName) const;
	SpawnPresets*    getSpawnPresets() const;

	sf::Time loadHighScore() const;
	void saveHighScore(sf::Time score) const;

private:
	std::unordered_map<std::string, sf::Font*> m_fonts;
	std::unordered_map<std::string, sf::Texture*> m_textures;
	std::unordered_map<std::string, sf::SoundBuffer*> m_soundBuffers;
	std::unordered_map<std::string, sf::Shader*> m_shaders;

	std::mutex m_fontsMutex;
	std::mutex m_texturesMutex;
	std::mutex m_soundsMutex;
	std::mutex m_shadersMutex;

	SpawnPresets* m_spawnPresets;
};

