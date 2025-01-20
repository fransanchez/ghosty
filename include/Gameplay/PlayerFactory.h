#pragma once

#include <Gameplay/Player.h>
#include <unordered_map>
#include <string>

class PlayerFactory
{
public:
    static Player* createPlayer(const std::string& basePath, const sf::Vector2f& position, const sf::Vector2f& speed);
};
