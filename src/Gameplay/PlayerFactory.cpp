#include <Gameplay/PlayerFactory.h>
#include <Core/AssetManager.h>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

Player* PlayerFactory::createPlayer(const std::string& configPath, const sf::Vector2f& position, const sf::Vector2f& speed)
{
    std::ifstream configFile(configPath);
    if (!configFile.is_open())
    {
        printf("Error: Could not open config file %s\n", configPath.c_str());
        return nullptr;
    }

    json animationConfig;
    configFile >> animationConfig;

    std::unordered_map<AnimationType, Animation> animations;

    for (const auto& [key, frames] : animationConfig.items())
    {
        AnimationType animType;
        if (key == "Idle") animType = AnimationType::Idle;
        else if (key == "Walk") animType = AnimationType::Walk;
        else if (key == "Run") animType = AnimationType::Run;
        else if (key == "Jump") animType = AnimationType::Jump;
        else if (key == "Attack") animType = AnimationType::Attack;
        else if (key == "Hurt") animType = AnimationType::Hurt;
        else if (key == "Die") animType = AnimationType::Die;
        else
            continue;

        Animation animation;
        for (const auto& framePath : frames)
        {
            sf::Texture* texture = AssetManager::getInstance()->loadTexture(framePath.get<std::string>().c_str());
            if (texture)
            {
                animation.addFrame(texture);
            }
            else
            {
                printf("Error: Could not load texture %s\n", framePath.get<std::string>().c_str());
            }
        }

        if (animType == AnimationType::Idle)
            animation.setFrameDuration(0.05f); // 20 FPS
        else if (animType == AnimationType::Walk)
            animation.setFrameDuration(0.08f); // 12.5 FPS
        else if (animType == AnimationType::Run)
            animation.setFrameDuration(0.06f); // 16.67 FPS

        animations[animType] = animation;
    }

    Player::PlayerDescriptor descriptor;
    descriptor.position = position;
    descriptor.speed = speed;

    Player* player = new Player();
    if (!player->init(descriptor, animations))
    {
        printf("Error: Could not initialize player\n");
        delete player;
        return nullptr;
    }

    return player;
}
