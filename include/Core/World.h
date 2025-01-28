#pragma once

#include <cstdint>
#include <Core/Level.h>
#include <Gameplay/Player/Player.h>
#include <SFML/Graphics/View.hpp>

class CollisionsManager;
class EnemyManager;
class HUD;

namespace sf
{
	class RenderWindow;
}

class World
{
	public:

		~World();

		bool load(uint32_t cameraWidth, uint32_t cameraHeight);
		void unload();

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

		bool isPlayerDead() const;

	private:
		void updateCamera();

		Level* m_level{};
		Player* m_player{ nullptr };
		CollisionManager* m_collisionManager{ nullptr };
		EnemyManager* m_enemyManager{ nullptr };
		HUD* m_hud{ nullptr };
		sf::View m_camera;
		sf::View m_uiView;
};