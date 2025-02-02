#pragma once

#include <Gameplay/Collectibles/Collectible.h>
#include <Utils/ObjectPool.h>
#include <unordered_map>
#include <vector>

class CollisionManager;

class CollectibleManager
{
public:
    CollectibleManager(CollisionManager* collisionManager);
    ~CollectibleManager();

    bool loadCollectiblesInRange(
        const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& spawnPoints,
        const sf::View& cameraView);
    void update(uint32_t deltaMilliseconds, const sf::View& cameraView);
    void render(sf::RenderWindow& window);
    void unload();

private:
    Collectible* getFromPool();
    void releaseToPool(Collectible* collectible);

    CollisionManager* m_collisionManager;
    std::unordered_map<CollectibleType, Animation*> m_collectibleAnimations;
    std::vector<Collectible*> m_activeCollectibles;
    std::vector<Collectible*> m_visibleCollectibles;
    std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>> m_spawnPoints;
    std::vector<sf::Vector2f> m_loadedSpawnPoints;

    ObjectPool<Collectible, 5> m_collectiblePool;
};
