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

    bool loadCollectibles(const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& spawnPoints);
    void update(uint32_t deltaMilliseconds);
    void render(sf::RenderWindow& window);
    void unload();

private:
    Collectible* getFromPool();
    void releaseToPool(Collectible* collectible);

    CollisionManager* m_collisionManager;
    std::unordered_map<CollectibleType, Animation*> m_collectibleAnimations;
    std::vector<Collectible*> m_activeCollectibles;

    ObjectPool<Collectible, 5> m_collectiblePool;
};
