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
    PlayerAssets assets = loadPlayerAssets(configPath);

    if (assets.animations.empty())
    {
        printf("Error: No animations loaded for player.\n");
        return nullptr;
    }

    Player::PlayerDescriptor descriptor;
    descriptor.position = position;
    descriptor.speed = speed;

    Player* player = new Player();
    if (!player->init(descriptor, assets.animations, std::move(assets.attacks)))
    {
        printf("Error: Could not initialize player.\n");
        delete player;
        return nullptr;
    }

    return player;
}

PlayerAssets PlayerFactory::loadPlayerAssets(const std::string& configPath)
{
    // Leer archivo JSON
    std::ifstream configFile(configPath);
    if (!configFile.is_open())
    {
        printf("Error: Could not open config file %s\n", configPath.c_str());
        return {};
    }

    json config;
    configFile >> config;

    std::unordered_map<AnimationType, Animation> animations;
    std::unordered_map<std::string, std::unique_ptr<Attack>> attacks;

    for (const auto& [key, animationData] : config["Animations"].items())
    {
        AnimationType animType = parseAnimationType(key);
        animations[animType] = loadAnimation(animationData);
    }

    if (config.contains("Attacks"))
    {
        for (const auto& [attackName, attackData] : config["Attacks"].items())
        {
            Animation attackAnimation = loadAnimation(attackData);
            float speed = attackData.contains("Speed") ? attackData["Speed"].get<float>() : 500.0f;
            float lifetime = attackData.contains("Lifetime") ? attackData["Lifetime"].get<float>() : 2.0f;
            attacks[attackName] = std::make_unique<ProjectileAttack>(attackAnimation, speed, lifetime);
        }
    }

    return { animations, std::move(attacks) };
}

Animation PlayerFactory::loadAnimation(const nlohmann::json& data)
{
    const std::string& texturePath = data["TexturePath"].get<std::string>();
    int frameRows = data["FrameCountRows"].get<int>();
    int frameColumns = data["FrameCountColumns"].get<int>();
    float frameDuration = data.contains("FrameDuration") ? data["FrameDuration"].get<float>() : 0.1f;
    bool loop = data.contains("Loop") ? data["Loop"].get<bool>() : true;

    sf::Texture* texture = AssetManager::getInstance()->loadTexture(texturePath.c_str());
    if (!texture)
    {
        printf("Error: Could not load texture %s\n", texturePath.c_str());
        return {};
    }

    int frameWidth = texture->getSize().x / frameColumns;
    int frameHeight = texture->getSize().y / frameRows;

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

    animation.setFrameDuration(frameDuration);
    animation.setLoop(loop);
    return animation;
}