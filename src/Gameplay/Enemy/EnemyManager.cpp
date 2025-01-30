#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/AttackSystem/MeleeAttack.h>
#include <Gameplay/AttackSystem/RangedAttack.h>
#include <Gameplay/Enemy/DinoEnemy.h>
#include <Gameplay/Enemy/EnemyManager.h>
#include <Gameplay/Enemy/EnemyFactory.h>
#include <Gameplay/Enemy/GhostEnemy.h>
#include <Gameplay/Enemy/SkeletonEnemy.h>
#include <Gameplay/Enemy/VampireEnemy.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <SFML/Graphics/RenderWindow.hpp>

EnemyManager::EnemyManager(CollisionManager* collisionManager)
    : m_collisionManager(collisionManager)
{
    for (EnemyType type : {EnemyType::Ghost, EnemyType::Skeleton, EnemyType::Dino, EnemyType::Vampire})
    {
        m_enemyDescriptors[type] = EnemyFactory::loadEnemyDescriptor(type, collisionManager);
    }
}

EnemyManager::~EnemyManager()
{
    unload();
}

bool EnemyManager::loadEnemies(const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& spawnPoints)
{
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

            Enemy* enemy = getFromPools(enemyType);
            if (!enemy)
            {
                printf("Error: Could not get enemy from pool for type '%s'.\n", typeString.c_str());
                continue;
            }

            if (!initEnemyFromCachedDescriptor(enemy, enemyType, position))
            {
                printf("Error: Could not initialize enemy of type '%s'.\n", typeString.c_str());
                releaseFromPools(enemyType, enemy);
            }
            else
            {
                m_activeEnemies.push_back(enemy);
                m_collisionManager->registerEnemy(enemy);
            }
        }
    }
    return true;
}

void EnemyManager::update(uint32_t deltaMilliseconds)
{
    for (auto it = m_activeEnemies.begin(); it != m_activeEnemies.end();)
    {
        Enemy* enemy = *it;
        if (enemy->isMarkedForDestruction())
        {
            m_collisionManager->unregisterEnemy(enemy);
            releaseFromPools(enemy->getType(), enemy);
            it = m_activeEnemies.erase(it);
        }
        else
        {
            enemy->update(deltaMilliseconds);
            ++it;
        }
    }
}

void EnemyManager::render(sf::RenderWindow& window)
{
    for (Enemy* enemy : m_activeEnemies)
    {
        enemy->render(window);
    }
}

Enemy* EnemyManager::getFromPools(EnemyType type)
{
    switch (type)
    {
    case EnemyType::Ghost:
        return &m_ghostPool.get();
    case EnemyType::Skeleton:
        return &m_skeletonPool.get();
    case EnemyType::Dino:
        return &m_dinoPool.get();
    case EnemyType::Vampire:
        return &m_vampirePool.get();
    default:
        printf("Error: Unsupported enemy type in getFromPools.\n");
        return nullptr;
    }
}

void EnemyManager::releaseFromPools(EnemyType type, Enemy* enemy)
{
    enemy->reset();

    switch (type)
    {
    case EnemyType::Ghost:
        m_ghostPool.release(*static_cast<GhostEnemy*>(enemy));
        break;
    case EnemyType::Skeleton:
        m_skeletonPool.release(*static_cast<SkeletonEnemy*>(enemy));
        break;
    case EnemyType::Dino:
        m_dinoPool.release(*static_cast<DinoEnemy*>(enemy));
        break;
    case EnemyType::Vampire:
        m_vampirePool.release(*static_cast<VampireEnemy*>(enemy));
        break;
    default:
        printf("Error: Unsupported enemy type in releaseFromPools.\n");
        break;
    }
}

void EnemyManager::unload()
{
    for (Enemy* enemy : m_activeEnemies)
    {
        m_collisionManager->unregisterEnemy(enemy);
        releaseFromPools(enemy->getType(), enemy);
    }
    m_activeEnemies.clear();

    // Delete cached copies of descriptors and attacks
    for (auto& [type, descriptor] : m_enemyDescriptors)
    {
        if (descriptor->animations)
        {
            for (auto& [type, animation] : *descriptor->animations)
            {
                delete animation;
            }
            delete descriptor->animations;
        }

        for (auto* attack : descriptor->attacks)
        {
            delete attack;
        }

        delete descriptor->collider;
        delete descriptor;
    }
    m_enemyDescriptors.clear();
}

bool EnemyManager::initEnemyFromCachedDescriptor(Enemy* enemy, EnemyType enemyType, sf::Vector2f position)
{
    Enemy::EnemyDescriptor cachedDescriptor = *m_enemyDescriptors[enemyType];

    Enemy::EnemyDescriptor descriptorCopy;
    descriptorCopy.position = position;
    descriptorCopy.patrolArea = m_collisionManager->getClosestPatrolArea(position);
    descriptorCopy.speed = cachedDescriptor.speed;
    descriptorCopy.sightArea = cachedDescriptor.sightArea;
    descriptorCopy.maxLife = cachedDescriptor.maxLife;
    descriptorCopy.type = cachedDescriptor.type;
    descriptorCopy.collider = new Collider(*cachedDescriptor.collider);
    descriptorCopy.animations = new std::unordered_map<AnimationType, Animation*>();
    descriptorCopy.animations = new std::unordered_map<AnimationType, Animation*>();
    for (const auto& [type, animation] : *cachedDescriptor.animations)
    {
        (*descriptorCopy.animations)[type] = new Animation(*animation);
    }
    for (const auto* attack : cachedDescriptor.attacks)
    {
        descriptorCopy.attacks.push_back(attack->clone());
    }

    return enemy->init(descriptorCopy, m_collisionManager);
}