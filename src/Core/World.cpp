#include <Core/World.h>
#include <Core/AssetManager.h>
#include <Gameplay/Collisions/Collider.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/Enemy.h>
#include <Gameplay/Enemy/EnemyFactory.h>
#include <Gameplay/Enemy/EnemyType.h>
#include <Gameplay/Player/PlayerFactory.h>

World::~World()
{
	unload();
}

bool World::load()
{
	constexpr float millisecondsToSeconds = 1 / 1000.f;


	m_level = new Level();
	if (!m_level->load(LEVEL_MAP_PATH))
	{
		return false;
	}

	// Create and initialize the CollisionManager
	m_collisionManager = new CollisionManager();
	m_collisionManager->setGroundShapes(m_level->getFloorsCollisionShapes());
	m_collisionManager->setWallShapes(m_level->getWallsCollisionShapes());
	m_collisionManager->setEnemyPatrolAreasShapes(m_level->getEnemyPatrolAreasShapes());

	std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>> playerSpawnPoint = m_level->getPlayerSpawnPoint();
	m_player = PlayerFactory::createPlayer(PLAYER_CONFIG_PATH, playerSpawnPoint.first, { 200.f, 150.f }, m_collisionManager);
	if (!m_player)
	{
		return false;
	}
	m_collisionManager->registerPlayer(m_player);

	const auto& spawnPoints = m_level->getEnemySpawnPoints();

	for (const auto& [position, properties] : spawnPoints)
	{
		auto it = properties.find("type");
		if (it != properties.end())
		{
			const std::string& typeString = it->second;
			EnemyType enemyType = stringToEnemyType(typeString);

			if (enemyType == EnemyType::Unknown)
			{
				printf("Warning: Unsupported enemy type '%s' at spawn point.\n", typeString.c_str());
				continue;
			}

			Enemy* enemy = EnemyFactory::createEnemy(enemyType, position, m_collisionManager);

			if (enemy)
			{
				m_collisionManager->registerEnemy(enemy);
				m_enemies.push_back(enemy);
			}
		}
	}

	return true;
}

void World::unload()
{
	m_collisionManager->unregisterPlayer();

	for (Enemy* enemy : m_enemies) {
		m_collisionManager->unregisterEnemy(enemy);
		delete enemy;
		enemy = nullptr;
	}
	m_enemies.clear();

	delete m_collisionManager;
	m_collisionManager = nullptr;

	delete m_player;
	m_player = nullptr;

	m_level->unload();
	delete m_level;
	m_level = nullptr;
}

void World::update(uint32_t deltaMilliseconds)
{
	m_level->update(deltaMilliseconds);

	if (m_player)
	{
		if (!m_player->isMarkedForDestruction()) {
			m_player->update(deltaMilliseconds);
		}
		else 
		{
			// To-Do: switch screen
			m_collisionManager->unregisterPlayer();
			delete(m_player);
			m_player = nullptr;
		}
	}
	// Update actors
	for (auto it = m_enemies.begin(); it != m_enemies.end(); )
	{
		if ((*it)->isMarkedForDestruction()) {
			delete* it;
			it = m_enemies.erase(it);
		}
		else {
			(*it)->update(deltaMilliseconds);
			++it;
		}
	}
}

void World::render(sf::RenderWindow& window)
{
	m_level->render(window);
	if (m_player) {
		m_player->render(window);
	}
	for (auto* enemy : m_enemies)
	{
		enemy->render(window);
	}
}


