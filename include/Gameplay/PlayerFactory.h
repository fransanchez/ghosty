#pragma once

#include <Gameplay/Player.h>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

struct PlayerAssets {
    std::unordered_map<AnimationType, Animation> animations;
    std::unordered_map<AttackAnimationType, AttackAnimation> attacks;
};

class PlayerFactory
{
    public:
        static Player* createPlayer(const std::string& basePath, const sf::Vector2f& position, const sf::Vector2f& speed);

    private:
        static PlayerAssets loadPlayerAssets(const std::string& configPath);
};
