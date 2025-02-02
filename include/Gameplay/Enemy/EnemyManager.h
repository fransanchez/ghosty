#pragma once

#include <Gameplay/Enemy/DinoEnemy.h>
#include <Gameplay/Enemy/Enemy.h>
#include <Gameplay/Enemy/EnemyFactory.h>
#include <Gameplay/Enemy/GhostEnemy.h>
#include <Gameplay/Enemy/SkeletonEnemy.h>
#include <Gameplay/Enemy/VampireEnemy.h>
#include <unordered_map>
#include <Utils/ObjectPool.h>
#include <vector>

class CollisionManager;

class EnemyManager
{
public:
    EnemyManager(CollisionManager* collisionManager);
    ~EnemyManager();

    bool loadEnemiesInRange(
        const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& spawnPoints,
        const sf::View& cameraView);

    void update(uint32_t deltaMilliseconds, const sf::View& cameraView);
    void render(sf::RenderWindow& window);
    void unload();

private:
    Enemy* getFromPools(EnemyType type);
    void releaseFromPools(EnemyType type, Enemy* enemy);
    bool initEnemyFromCachedDescriptor(Enemy* enemy, EnemyType enemyType, sf::Vector2f position);

    CollisionManager* m_collisionManager;
    std::unordered_map<EnemyType, Enemy::EnemyDescriptor*> m_enemyDescriptors; // Cached descriptors so we don't have to load from file each time
    std::vector<Enemy*> m_activeEnemies;
    std::vector<Enemy*> m_visibleEnemies;
    std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>> m_spawnPoints;
    std::vector<sf::Vector2f> m_loadedSpawnPoints;

    ObjectPool<GhostEnemy, 3> m_ghostPool;
    ObjectPool<SkeletonEnemy, 3> m_skeletonPool;
    ObjectPool<DinoEnemy, 3> m_dinoPool;
    ObjectPool<VampireEnemy, 3> m_vampirePool;
};