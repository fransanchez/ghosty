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
    // Json config
    std::ifstream configFile(configPath);
    if (!configFile.is_open())
    {
        printf("Error: Could not open config file %s\n", configPath.c_str());
        return nullptr;
    }

    json animationConfig;
    configFile >> animationConfig;

    std::unordered_map<AnimationType, Animation> animations;

    for (const auto& [key, animationData] : animationConfig["Animations"].items())
    {
        AnimationType animType;
        if (key == "Idle") animType = AnimationType::Idle;
        else if (key == "Walk") animType = AnimationType::Walk;
        else if (key == "Run") animType = AnimationType::Run;
        else
            continue;

        const std::string& texturePath = animationData["TexturePath"].get<std::string>();
        int frameRows = animationData["FrameCountRows"].get<int>();
        int frameColumns = animationData["FrameCountColumns"].get<int>();

        sf::Texture* texture = AssetManager::getInstance()->loadTexture(texturePath.c_str());
        if (!texture)
        {
            printf("Error: Could not load texture %s\n", texturePath.c_str());
            continue;
        }

        // Break down the tilesheets and load each animation on a frame
        int textureWidth = texture->getSize().x;
        int textureHeight = texture->getSize().y;
        int frameWidth = textureWidth / frameColumns;
        int frameHeight = textureHeight / frameRows;

        Animation animation;
        for (int row = 0; row < frameRows; ++row)
        {
            for (int col = 0; col < frameColumns; ++col)
            {
                sf::Texture* frameTexture = new sf::Texture();
                if (frameTexture->loadFromImage(
                    texture->copyToImage(),
                    sf::IntRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight)))
                {
                    animation.addFrame(frameTexture);
                }
                else
                {
                    delete frameTexture;
                    printf("Error: Could not extract frame (%d, %d) from %s\n", row, col, texturePath.c_str());
                }
            }
        }

        float frameDuration = animationConfig["FrameDuration"].contains(key)
            ? animationConfig["FrameDuration"][key].get<float>()
            : 0.1f;
        animation.setFrameDuration(frameDuration);

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
