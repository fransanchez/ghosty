#include <Core/World.h>
#include <Core/AssetManager.h>
#include <Gameplay/Collider.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/Enemy.h>
#include <Gameplay/Enemy/EnemyFactory.h>
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

	sf::Vector2f playerSpawnPoint = m_level->getPlayerSpawnPoint();
	m_player = PlayerFactory::createPlayer(PLAYER_CONFIG_PATH, playerSpawnPoint, { 200.f, 150.f }, m_collisionManager);
	if (!m_player)
	{
		return false;
	}

	// Load Enemy
	sf::Vector2f enemySpawnPoint = m_level->getEnemySpawnPoints().empty() ? sf::Vector2f(300.f, 300.f) : m_level->getEnemySpawnPoints()[0];
	m_enemy = EnemyFactory::createEnemy(GHOST_ENEMY_CONFIG_PATH, enemySpawnPoint, m_collisionManager);
	if (!m_enemy)
	{
		return false;
	}

	return true;
}

void World::unload()
{
	delete m_collisionManager;
	m_collisionManager = nullptr;

	delete m_player;
	m_player = nullptr;

	delete m_enemy;
	m_enemy = nullptr;

	m_level->unload();
	delete m_level;
	m_level = nullptr;
}

void World::update(uint32_t deltaMilliseconds)
{
	m_level->update(deltaMilliseconds);

	// Update actors
	m_player->update(deltaMilliseconds);

	m_enemy->update(deltaMilliseconds);

}

void World::render(sf::RenderWindow& window)
{
	m_level->render(window);
	m_player->render(window);
	m_enemy->render(window);
}


