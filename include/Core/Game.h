#pragma once

#include <cstdint>
#include <string>

namespace sf
{
	class RenderWindow;
}

class World;
class UIManager;
class UIScreen;

class Game
{
	public:

		struct GameCreateInfo
		{
			std::string gameTitle;
			uint32_t screenWidth;
			uint32_t screenHeight;
			uint32_t frameRateLimit;
			std::string windowIconPath;
		};

		~Game();

		bool init(GameCreateInfo& createInfo);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

	private:

		UIManager* m_uiManager{ nullptr } ;
		UIScreen* m_mainMenu{ nullptr };
		sf::RenderWindow* m_window{ nullptr };
		World* m_world{ nullptr };
};