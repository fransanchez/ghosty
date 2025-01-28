#include <Utils/GameConfigLoader.h>
#include <fstream>
#include <iostream>

nlohmann::json GameConfigLoader::loadConfig(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open config file: " << filePath << std::endl;
        return {};
    }

    nlohmann::json config;
    file >> config;
    return config;
}