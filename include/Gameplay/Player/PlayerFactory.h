#pragma once

#include <Gameplay/Player/Player.h>
#include <vector>
#include <memory>

class PlayerFactory
{
    public:
        static Player* createPlayer(const std::string& basePath, const sf::Vector2f& position, const sf::Vector2f& speed);
    
    private:
        static std::vector<std::unique_ptr<Attack>> loadAttacks(const std::string& configPath);
};
