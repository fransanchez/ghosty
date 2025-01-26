#include <Core/World.h>
#include <Core/AssetManager.h>
#include <Gameplay/Collider.h>
#include <Gameplay/CollisionManager.h>
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
				m_enemies.push_back(enemy);
			}
		}
	}

	return true;
}

void World::unload()
{
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

	// Update actors
	m_player->update(deltaMilliseconds);

	for (auto* enemy : m_enemies)
	{
		enemy->update(deltaMilliseconds);
	}
}

void World::render(sf::RenderWindow& window)
{
	m_level->render(window);
	m_player->render(window);
	for (auto* enemy : m_enemies)
	{
		enemy->render(window);
	}
}


