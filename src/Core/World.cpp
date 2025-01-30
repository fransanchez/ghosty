#include <Core/World.h>
#include <Core/AssetManager.h>
#include <Gameplay/Collectibles/CollectibleManager.h>
#include <Gameplay/Collisions/Collider.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/Enemy.h>
#include <Gameplay/Enemy/EnemyFactory.h>
#include <Gameplay/Enemy/EnemyManager.h>
#include <Gameplay/Enemy/EnemyType.h>
#include <Gameplay/Player/PlayerFactory.h>
#include <UI/HUD.h>

World::~World()
{
	unload();
}

bool World::load(uint32_t cameraWidth, uint32_t cameraHeight)
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

	sf::Vector2f playerPosition = m_player->getPosition();
	m_camera.setSize(cameraWidth, cameraHeight);
	m_camera.setCenter(playerPosition);

	m_uiView = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(cameraWidth), static_cast<float>(cameraHeight)));

	m_enemyManager = new EnemyManager(m_collisionManager);
	if (!m_enemyManager->loadEnemies(m_level->getEnemySpawnPoints()))
	{
		printf("Failed to load enemies.\n");
		return false;
	}

	m_collectibleManager = new CollectibleManager(m_collisionManager);
	if (!m_collectibleManager->loadCollectibles(m_level->getCollectiblesSpawnPoints()))
	{
		printf("Failed to load collectibles.\n");
		return false;
	}

	m_hud = new HUD();
	if (!m_hud->init(m_player->getMaxLives()))
	{
		printf("Error: Could not initialize HUD.\n");
		return false;
	}
	return true;
}

void World::unload()
{
	delete m_enemyManager;
	m_enemyManager = nullptr;

	delete m_collectibleManager;
	m_collectibleManager = nullptr;

	m_collisionManager->unregisterPlayer();

	delete m_player;
	m_player = nullptr;

	delete m_collisionManager;
	m_collisionManager = nullptr;

	delete m_level;
	m_level = nullptr;

	delete m_hud;
	m_hud = nullptr;
}

void World::update(uint32_t deltaMilliseconds)
{

	if (m_player)
	{
		if (!m_player->isMarkedForDestruction()) {
			m_level->update(deltaMilliseconds);
			m_player->update(deltaMilliseconds);
			if (m_enemyManager)
			{
				m_enemyManager->update(deltaMilliseconds);
			}

			if (m_collectibleManager)
			{
				m_collectibleManager->update(deltaMilliseconds);
			}

			updateCamera();

			if (m_player && m_hud)
			{
				m_hud->update(m_player->getCurrentLives());
			}
		}
	}
}

void World::render(sf::RenderWindow& window)
{
	window.setView(m_camera);

	m_level->render(window);
	if (m_player) {
		m_player->render(window);
	}
	m_enemyManager->render(window);

	if (m_collectibleManager)
	{
		m_collectibleManager->render(window);
	}

	if (m_hud)
	{
		const sf::View originalView = window.getView();
		// We change the view briefly to render hud and go back to original.
		// This ensures our HUD is always on top of the current camera
		window.setView(m_uiView);
		m_hud->render(window);
		window.setView(originalView);
	}
}

void World::updateCamera()
{
	if (!m_player)
		return;

	sf::Vector2f playerPosition = m_player->getPosition();

	float levelWidth = m_level->getWidth();
	float levelHeight = m_level->getHeight();
	sf::Vector2f viewSize = m_camera.getSize();

	float leftBound = viewSize.x / 2.f;
	float rightBound = levelWidth - viewSize.x / 2.f;

	float topBound = (levelHeight > viewSize.y) ? (viewSize.y / 2.f) : (levelHeight / 2.f);
	float bottomBound = (levelHeight > viewSize.y) ? (levelHeight - viewSize.y / 2.f) : (levelHeight / 2.f);

	float x = std::clamp(playerPosition.x, leftBound, rightBound);
	float y = std::clamp(playerPosition.y, topBound, bottomBound);

	if (levelHeight <= viewSize.y)
	{
		y = levelHeight / 2.f;
	}

	m_camera.setCenter(x, y);
}

bool World::isPlayerDead() const
{
	return m_player && m_player->isMarkedForDestruction();
}