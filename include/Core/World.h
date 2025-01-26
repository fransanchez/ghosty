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

		// TO-DO: Ideally the scene should be read from file.
		bool load();
		void unload();

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

	//	ObjectPool<Zombie, 10> m_zombiesPool;
		std::vector<Enemy*> m_enemies;
		// This is just an example. Think a good way to group the actors of your game. If they need any type of manager, etc...
		Level* m_level{};
		Player* m_player{ nullptr };
		CollisionManager* m_collisionManager{ nullptr };
};