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

    bool loadEnemies(const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& spawnPoints);
    void update(uint32_t deltaMilliseconds);
    void render(sf::RenderWindow& window);
    void unload();

private:
    Enemy* getFromPools(EnemyType type);
    void releaseFromPools(EnemyType type, Enemy* enemy);
    bool initEnemyFromCachedDescriptor(Enemy* enemy, EnemyType enemyType, sf::Vector2f position);

    CollisionManager* m_collisionManager;
    std::unordered_map<EnemyType, Enemy::EnemyDescriptor*> m_enemyDescriptors; // Cached descriptors so we don't have to load from file each time
    std::vector<Enemy*> m_activeEnemies;

    ObjectPool<GhostEnemy, 4> m_ghostPool;
    ObjectPool<SkeletonEnemy, 4> m_skeletonPool;
    ObjectPool<DinoEnemy, 4> m_dinoPool;
    ObjectPool<VampireEnemy, 4> m_vampirePool;
};