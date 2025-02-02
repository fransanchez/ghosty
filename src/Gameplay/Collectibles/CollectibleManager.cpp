#include <Gameplay/Collectibles/CollectibleManager.h>
#include <Gameplay/Collectibles/CollectibleFactory.h>
#include <Gameplay/Collectibles/CollectibleType.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Utils/Constants.h>

CollectibleManager::CollectibleManager(CollisionManager* collisionManager)
    : m_collisionManager(collisionManager)
{
    m_collectibleAnimations = CollectibleFactory::loadCollectibleAnimations();
}

CollectibleManager::~CollectibleManager()
{
    unload();
}

bool CollectibleManager::loadCollectiblesInRange(
    const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& spawnPoints,
    const sf::View& cameraView)
{
    sf::FloatRect preloadBounds;
    sf::Vector2f center = cameraView.getCenter();
    sf::Vector2f size = cameraView.getSize();
    preloadBounds.left = center.x - size.x / 2;
    preloadBounds.top = center.y - size.y / 2;
    preloadBounds.width = size.x + PRELOAD_ADVANCE_RANGE;
    preloadBounds.height = size.y;

    m_spawnPoints = spawnPoints;

    for (const auto& [position, properties] : spawnPoints)
    {

        if (std::find(m_loadedSpawnPoints.begin(), m_loadedSpawnPoints.end(), position) != m_loadedSpawnPoints.end())
        {
            continue;
        }

        if (preloadBounds.contains(position))
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
                    m_loadedSpawnPoints.push_back(position);
                }
            }
        }
    }
    return true;
}

void CollectibleManager::update(uint32_t deltaMilliseconds, const sf::View& cameraView)
{
    loadCollectiblesInRange(m_spawnPoints, cameraView);

    m_visibleCollectibles.clear();
    for (auto it = m_activeCollectibles.begin(); it != m_activeCollectibles.end();)
    {
        Collectible* collectible = *it;

        sf::FloatRect renderBounds;
        sf::Vector2f center = cameraView.getCenter();
        sf::Vector2f size = cameraView.getSize();
        renderBounds.left = center.x - size.x / 2;
        renderBounds.top = center.y - size.y / 2;
        renderBounds.width = size.x;
        renderBounds.height = size.y;
        if (renderBounds.contains(collectible->getPosition()))
        {
            m_visibleCollectibles.push_back(collectible);
        }

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
    for (Collectible* collectible : m_visibleCollectibles)
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

    m_visibleCollectibles.clear();

    m_spawnPoints.clear();
    m_loadedSpawnPoints.clear();

    for (auto& [type, animation] : m_collectibleAnimations)
    {
        delete animation;
    }
    m_collectibleAnimations.clear();
}
