#include <Core/AssetManager.h>
#include <fstream>
#include <Gameplay/Collisions/Collider.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/Enemy.h>
#include <Gameplay/Enemy/EnemyFactory.h>
#include <Gameplay/Enemy/EnemyType.h>
#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/AttackSystem/RangedAttack.h>
#include <Gameplay/AttackSystem/MeleeAttack.h>
#include <nlohmann/json.hpp>
#include <Render/Animation.h>
#include <Render/AnimationLoader.h>
#include <Render/AnimationType.h>
#include <sstream>
#include <Utils/Constants.h>

using json = nlohmann::json;

Enemy::EnemyDescriptor* EnemyFactory::loadEnemyDescriptor(const EnemyType enemyType, CollisionManager* collisionManager)
{
    static const std::unordered_map<EnemyType, std::string> enemyConfigPaths = {
        { EnemyType::Ghost, GHOST_ENEMY_CONFIG_PATH },
        { EnemyType::Skeleton, SKELETON_ENEMY_CONFIG_PATH },
        { EnemyType::Dino, DINO_ENEMY_CONFIG_PATH },
        { EnemyType::Vampire, VAMPIRE_ENEMY_CONFIG_PATH }
    };

    auto it = enemyConfigPaths.find(enemyType);
    if (it == enemyConfigPaths.end())
    {
        printf("Error: Unsupported EnemyType\n");
        throw std::runtime_error("Invalid enemy type");
    }

    const std::string& configPath = it->second;

    std::ifstream file(configPath);
    if (!file.is_open())
    {
        printf("Error: Could not open enemy configuration file %s\n", configPath.c_str());
        throw std::runtime_error("Failed to open config file");
    }

    json config;
    file >> config;

    auto animations = loadAnimations(config);
    auto attacks = loadAttacks(config, collisionManager);
    // Load collider (position will be set later)
    auto collider = loadCollider(config, { 0.f, 0.f });

    sf::Vector2f speed = { 0.f, 0.f }; // Default speed
    if (config.contains("Speed"))
    {
        speed.x = config["Speed"]["X"].get<float>();
        speed.y = config["Speed"]["Y"].get<float>();
    }

    sf::Vector2f sightArea = { 300.f, 150.f }; // Default sight area
    if (config.contains("SightArea"))
    {
        sightArea.x = config["SightArea"]["Width"].get<float>();
        sightArea.y = config["SightArea"]["Height"].get<float>();
    }

    int maxLife = config.contains("MaxLife") ? config["MaxLife"].get<int>() : 1;

    Enemy::EnemyDescriptor* descriptor = new Enemy::EnemyDescriptor();
    descriptor->speed = speed;
    descriptor->sightArea = sightArea;
    descriptor->animations = new std::unordered_map<AnimationType, Animation*>(std::move(animations));
    descriptor->attacks = attacks;
    descriptor->maxLife = maxLife;
    descriptor->type = enemyType;
    descriptor->collider = collider;

    return descriptor;
}

std::unordered_map<AnimationType, Animation*> EnemyFactory::loadAnimations(const json& config)
{
    return AnimationLoader::LoadAnimations(config["Animations"]);
}

std::vector<Attack*> EnemyFactory::loadAttacks(const json& config, CollisionManager* collisionManager)
{
    std::vector<Attack*> attacks;

    if (config.contains("Attacks"))
    {
        for (const auto& [attackName, attackData] : config["Attacks"].items())
        {
            float damage = attackData["Damage"].get<float>();
            float lifetime = attackData["LifetimeMilliseconds"].get<float>();

            // Load collider for the attack
            Collider* attackCollider = loadCollider(attackData, { 0.f, 0.f });
            if (!attackCollider)
            {
                printf("Warning: Collider not found for attack %s\n", attackName.c_str());
                continue;
            }

            // Create melee or ranged attack
            if (attackName == "Melee")
            {
                float attackRate = attackData.contains("AttackRatePerSecond") ? attackData["AttackRatePerSecond"].get<float>() : 1.0f;
                attacks.push_back(new MeleeAttack(AttackFaction::Enemy, damage, lifetime, attackRate, attackCollider, collisionManager));
            }
            else if (attackName == "Ranged")
            {
                Animation* animation = nullptr;
                if (attackData.contains("Animation"))
                {
                    const auto& animData = attackData["Animation"].begin().value();
                    animation = AnimationLoader::LoadSingleAttackAnimation(animData);
                }
                if (!animation)
                {
                    delete attackCollider;
                    printf("Warning: Animation not found for ranged attack %s\n", attackName.c_str());
                    continue;
                }

                float speed = attackData["Speed"].get<float>();
                float fireRate = attackData["FireRateSeconds"].get<float>();
                float range = attackData["Range"].get<float>();

                attacks.push_back(new RangedAttack(
                    AttackFaction::Enemy,
                    damage,
                    animation,
                    lifetime,
                    speed,
                    fireRate,
                    range,
                    attackCollider,
                    collisionManager));
            }
            else
            {
                delete attackCollider;
                printf("Warning: Unsupported attack type %s\n", attackName.c_str());
            }
        }
    }

    return attacks;
}

Collider* EnemyFactory::loadCollider(const json& parentData, const sf::Vector2f& position)
{
    if (!parentData.contains("Collider"))
    {
        printf("Warning: No collider data found.\n");
        return nullptr;
    }

    const auto& colliderData = parentData["Collider"];

    sf::Vector2f size{
        colliderData["Size"]["Width"].get<float>(),
        colliderData["Size"]["Height"].get<float>()
    };
    sf::Vector2f centerOffset{
        colliderData["CenterOffset"]["X"].get<float>(),
        colliderData["CenterOffset"]["Y"].get<float>()
    };

    return new Collider(position, size, centerOffset);
}
