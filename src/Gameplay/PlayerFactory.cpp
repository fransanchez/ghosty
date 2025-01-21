#include <Gameplay/PlayerFactory.h>
#include <Core/AssetManager.h>
#include <Render/Animation.h>
#include <Render/AnimationLoader.h>
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
    if (!player->init(descriptor, assets.animations, assets.attacks))
    {
        printf("Error: Could not initialize player.\n");
        delete player;
        return nullptr;
    }

    return player;
}

PlayerAssets PlayerFactory::loadPlayerAssets(const std::string& configPath)
{
    PlayerAssets assets;
    assets.animations = AnimationLoader::LoadPlayerAnimations(configPath);
    assets.attacks = AnimationLoader::LoadAttackAnimations(configPath);
    return assets;
}