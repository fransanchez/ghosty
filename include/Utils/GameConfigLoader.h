#pragma once

#include <nlohmann/json.hpp>
#include <string>

class GameConfigLoader
{
public:
    static nlohmann::json loadConfig(const std::string& filePath);
    static sf::Uint32 GameConfigLoader::getWindowStyleFromString(const std::string& styleStr);
};
