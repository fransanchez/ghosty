#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

class GameConfigLoader
{
public:
    static nlohmann::json loadConfig(const std::string& filePath);
    static sf::Vector2f getCameraSize(const nlohmann::json& config);
};
