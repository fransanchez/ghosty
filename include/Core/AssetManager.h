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

		sf::Texture* loadTexture(const char* assetPath);
		void unloadTexture(sf::Texture* texture);
		void clear();

		sf::Texture* getTexture(const char* assetPath) const;


	private:

		static AssetManager* s_instance;

		std::unordered_map<std::string, sf::Texture*> m_texturePathToTexture;
};