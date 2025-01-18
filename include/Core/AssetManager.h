#pragma once

#include <string>
#include <unordered_map>

namespace sf
{
	class Texture;
}

class AssetManager
{
	public:

		static AssetManager* getInstance();

		~AssetManager();

		// To-Do: Implement a clear()

		sf::Texture* loadTexture(const char* assetPath);
		// To-Do: Implement a unloadTexture()

		sf::Texture* getTexture(const char* assetPath) const;


	private:

		static AssetManager* s_instance;

		std::unordered_map<std::string, sf::Texture*> m_texturePathToTexture;
};