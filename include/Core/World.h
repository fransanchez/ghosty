#pragma once

#include <cstdint>
#include <Core/Level.h>
#include <Gameplay/Player/Player.h>
#include <Utils/ObjectPool.h>

class CollisionsManager;
class Enemy;

namespace sf
{
	class RenderWindow;
}

class World
{
	public:

		~World();

		bool load();
		void unload();

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

		std::vector<Enemy*> m_enemies;
		Level* m_level{};
		Player* m_player{ nullptr };
		CollisionManager* m_collisionManager{ nullptr };
};