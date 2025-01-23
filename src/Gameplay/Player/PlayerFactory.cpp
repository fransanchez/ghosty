#include <Core/AssetManager.h>
#include <fstream>
#include <Gameplay/Collider.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/AttackSystem/RangedAttack.h>
#include <Gameplay/Player/PlayerFactory.h>
#include <nlohmann/json.hpp>
#include <Render/Animation.h>
#include <Render/AnimationLoader.h>
#include <Render/AnimationType.h>
#include <sstream>


using json = nlohmann::json;

Player* PlayerFactory::createPlayer(const std::string& configPath,
                                    const sf::Vector2f& position,
                                    const sf::Vector2f& speed,
                                    CollisionManager* collisionManager)
{
    std::ifstream file(configPath);
    if (!file.is_open())
    {
        printf("Error: Could not open configuration file %s\n", configPath.c_str());
        return nullptr;
    }

    json config;
    file >> config;

    // Load player movement animations
    std::unordered_map<AnimationType, Animation*> playerAnimations = AnimationLoader::LoadPlayerAnimations(config);

    if (playerAnimations.empty())
    {
        printf("Error: No animations loaded for player.\n");
        return nullptr;
    }

    // Load Player Attacks
    std::vector<std::unique_ptr<Attack>> attacks = loadAttacks(config);

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

    std::unique_ptr<Collider> collider = loadCollider(config, player, position);
    if (collider)
    {
        player->setCollider(std::move(collider));
    }

    player->setCollisionManager(collisionManager);
    collisionManager->registerCollider(player->getCollider());

    return player;
}


std::vector<std::unique_ptr<Attack>> PlayerFactory::loadAttacks(const json& config)
{

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
                Animation* attackAnimation = AnimationLoader::LoadSingleAttackAnimation(animationData);

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

std::unique_ptr<Collider> PlayerFactory::loadCollider(const json& config, Player* player, const sf::Vector2f& position)
{
    if (!config.contains("Collider"))
    {
        printf("Warning: No collider data found in config.\n");
        return nullptr;
    }

    const auto& colliderData = config["Collider"];
    sf::FloatRect spriteBounds = player->getSpriteBounds();

    sf::Vector2f size{
        colliderData["Size"]["Width"].get<float>(),
        colliderData["Size"]["Height"].get<float>()
    };
    sf::Vector2f centerOffset{
        colliderData["CenterOffset"]["X"].get<float>(),
        colliderData["CenterOffset"]["Y"].get<float>()
    };

    // Align the collider relative to the center of the sprite
    return std::make_unique<Collider>(position, size, centerOffset);
}