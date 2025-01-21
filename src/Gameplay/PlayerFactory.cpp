#include <Core/AssetManager.h>
#include <fstream>
#include <Gameplay/PlayerFactory.h>
#include <Gameplay/Attack.h>
#include <Gameplay/RangedAttack.h>
#include <nlohmann/json.hpp>
#include <Render/Animation.h>
#include <Render/AnimationLoader.h>
#include <Render/AnimationType.h>
#include <sstream>


using json = nlohmann::json;

Player* PlayerFactory::createPlayer(const std::string& configPath, const sf::Vector2f& position, const sf::Vector2f& speed)
{

    // Load player movement animations
    std::unordered_map<AnimationType, Animation> playerAnimations = AnimationLoader::LoadPlayerAnimations(configPath);

    if (playerAnimations.empty())
    {
        printf("Error: No animations loaded for player.\n");
        return nullptr;
    }

    // Load Player Attacks
    std::vector<std::unique_ptr<Attack>> attacks = loadAttacks(configPath);

    if (attacks.empty())
    {
        printf("Error: No attacks loaded for player.\n");
        return nullptr;
    }

    Player::PlayerDescriptor descriptor;
    descriptor.position = position;
    descriptor.speed = speed;

    Player* player = new Player();
    if (!player->init(descriptor, playerAnimations, std::move(attacks)))
    {
        printf("Error: Could not initialize player.\n");
        delete player;
        return nullptr;
    }

    return player;
}


std::vector<std::unique_ptr<Attack>> PlayerFactory::loadAttacks(const std::string& configPath)
{
    std::ifstream file(configPath);
    if (!file.is_open())
    {
        printf("Error: Could not open configuration file %s\n", configPath.c_str());
        return {};
    }

    json config;
    file >> config;

    std::vector<std::unique_ptr<Attack>> attacks;

    if (config.contains("Attacks"))
    {
        for (const auto& [attackName, attackData] : config["Attacks"].items())
        {
            float damage = attackData["Damage"].get<float>();
            float lifetime = attackData["Lifetime"].get<float>();
            float speed = attackData["Speed"].get<float>();
            float fireRate = attackData["FireRate"].get<float>();

            if (attackData.contains("Animation"))
            {
                const auto& animationData = attackData["Animation"].begin().value();
                Animation attackAnimation = AnimationLoader::LoadSingleAttackAnimation(animationData);

                auto rangedAttack = std::make_unique<RangedAttack>(
                    damage,
                    attackAnimation,
                    lifetime,
                    speed,
                    fireRate
                );

                attacks.push_back(std::move(rangedAttack));
            }
            else
            {
                printf("Warning: No animation found for attack %s\n", attackName.c_str());
            }
        }
    }

    return attacks;
}