#include <fstream>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Utils/GameConfigLoader.h>

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

sf::Uint32 GameConfigLoader::getWindowStyleFromString(const std::string& styleStr)
{
    if (styleStr == "FullScreen")
    {
        return sf::Style::Fullscreen;
    }
    else if (styleStr == "Resize")
    {
        return sf::Style::Resize;
    }
    else if (styleStr == "Close")
    {
        return sf::Style::Close;
    }
    else
    {
        return sf::Style::Titlebar | sf::Style::Close;
    }
}