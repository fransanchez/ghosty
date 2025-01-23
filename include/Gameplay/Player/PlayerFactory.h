#pragma once

#include <Gameplay/Player/Player.h>
#include <vector>
#include <memory>

class PlayerFactory
{
    public:
        static Player* createPlayer(const std::string& basePath, const sf::Vector2f& position, const sf::Vector2f& speed, CollisionManager* collisionManager);
    
    private:
        static std::vector<std::unique_ptr<Attack>> loadAttacks(const nlohmann::json& config);
        static std::unique_ptr<Collider> PlayerFactory::loadCollider(const nlohmann::json& config, Player* player, const sf::Vector2f& position);

};
