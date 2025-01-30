#include <Gameplay/Collectibles/CollectibleManager.h>
#include <Gameplay/Collectibles/CollectibleFactory.h>
#include <Gameplay/Collectibles/CollectibleType.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <SFML/Graphics/RenderWindow.hpp>

CollectibleManager::CollectibleManager(CollisionManager* collisionManager)
    : m_collisionManager(collisionManager)
{
    m_collectibleAnimations = CollectibleFactory::loadCollectibleAnimations();
}

CollectibleManager::~CollectibleManager()
{
    unload();
}

bool CollectibleManager::loadCollectibles(const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& spawnPoints)
{
    for (const auto& [position, properties] : spawnPoints)
    {
        auto it = properties.find("type");
        if (it != properties.end())
        {
            const std::string& typeString = it->second;
            CollectibleType collectibleType = parseCollectibleType(typeString);

            if (collectibleType == CollectibleType::None)
            {
                printf("Warning: Unsupported collectible type '%s' at spawn point.\n", typeString.c_str());
                continue;
            }

            Collectible* collectible = getFromPool();
            if (!collectible)
            {
                printf("Error: Could not get collectible from pool for type '%s'.\n", typeString.c_str());
                continue;
            }

            Animation* animationCopy = new Animation(*m_collectibleAnimations[collectibleType]);
            if (!collectible->init(collectibleType, animationCopy, m_collisionManager, position))
            {
                printf("Error: Could not initialize collectible of type '%s'.\n", typeString.c_str());
                releaseToPool(collectible);
            }
            else
            {
                m_activeCollectibles.push_back(collectible);
                m_collisionManager->registerCollectible(collectible);
            }
        }
    }
    return true;
}

void CollectibleManager::update(uint32_t deltaMilliseconds)
{
    for (auto it = m_activeCollectibles.begin(); it != m_activeCollectibles.end();)
    {
        Collectible* collectible = *it;
        if (collectible->isMarkedForDestruction())
        {
            m_collisionManager->unregisterCollectible(collectible);
            releaseToPool(collectible);
            it = m_activeCollectibles.erase(it);
        }
        else
        {
            collectible->update(deltaMilliseconds);
            ++it;
        }
    }
}

void CollectibleManager::render(sf::RenderWindow& window)
{
    for (Collectible* collectible : m_activeCollectibles)
    {
        collectible->render(window);
    }
}

Collectible* CollectibleManager::getFromPool()
{
    return &m_collectiblePool.get();
}

void CollectibleManager::releaseToPool(Collectible* collectible)
{
    collectible->reset();
    m_collectiblePool.release(*collectible);
}

void CollectibleManager::unload()
{
    for (Collectible* collectible : m_activeCollectibles)
    {
        m_collisionManager->unregisterCollectible(collectible);
        releaseToPool(collectible);
    }
    m_activeCollectibles.clear();

    for (auto& [type, animation] : m_collectibleAnimations)
    {
        delete animation;
    }
    m_collectibleAnimations.clear();
}
