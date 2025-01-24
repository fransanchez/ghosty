#pragma once

#include <Gameplay/Player/Player.h>
#include <vector>
#include <memory>

class PlayerFactory
{
    public:
        static Player* createPlayer(const std::string& basePath, const sf::Vector2f& position, const sf::Vector2f& speed, CollisionManager* collisionManager);
    
    private:
        static std::vector<Attack*> loadAttacks(const nlohmann::json& config);
        static Collider* PlayerFactory::loadCollider(const nlohmann::json& parentData, const sf::Vector2f& position);
};
