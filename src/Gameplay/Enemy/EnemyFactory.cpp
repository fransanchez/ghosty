#include <Core/AssetManager.h>
#include <fstream>
#include <Gameplay/Collider.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/Enemy.h>
#include <Gameplay/Enemy/EnemyFactory.h>
#include <Gameplay/Enemy/GhostEnemy.h>
#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/AttackSystem/RangedAttack.h>
#include <Gameplay/AttackSystem/MeleeAttack.h>
#include <nlohmann/json.hpp>
#include <Render/Animation.h>
#include <Render/AnimationLoader.h>
#include <Render/AnimationType.h>
#include <sstream>

using json = nlohmann::json;

Enemy* EnemyFactory::createEnemy(const std::string& configPath, const sf::Vector2f& position, CollisionManager* collisionManager)
{
    std::ifstream file(configPath);
    if (!file.is_open())
    {
        printf("Error: Could not open enemy configuration file %s\n", configPath.c_str());
        return nullptr;
    }

    json config;
    file >> config;

    std::string enemyType = config["EnemyType"].get<std::string>();

    // Load animations
    auto animations = loadAnimations(config);
    if (animations.empty())
    {
        printf("Error: No animations loaded for enemy.\n");
        return nullptr;
    }

    // Load attacks
    auto attacks = loadAttacks(config);
    if (attacks.empty())
    {
        printf("Warning: No attacks loaded for enemy.\n");
    }

    // Load collider
    auto collider = loadCollider(config, position);
    if (!collider)
    {
        printf("Error: Could not load collider for enemy.\n");
        for (auto& [type, animation] : animations)
            delete animation;
        for (auto& attack : attacks)
            delete attack;
        return nullptr;
    }

    sf::Vector2f speed = { 0.f, 0.f }; // Default speed
    if (config.contains("Speed"))
    {
        speed.x = config["Speed"]["X"].get<float>();
        speed.y = config["Speed"]["Y"].get<float>();
    }

    // Create enemy based on type
    Enemy* enemy = nullptr;
    if (enemyType == "Ghost")
    {
        enemy = new GhostEnemy();
    }
    else
    {
        printf("Error: Unsupported EnemyType %s\n", enemyType.c_str());
        delete collider;
        for (auto& [type, animation] : animations)
            delete animation;
        for (auto& attack : attacks)
            delete attack;
        return nullptr;
    }
    Enemy::EnemyDescriptor descriptor;
    descriptor.position = position;
    descriptor.speed = speed;
    descriptor.animations = new std::unordered_map<AnimationType, Animation*>(std::move(animations));
    descriptor.attacks = attacks;

    if (!enemy->init(descriptor, collider, collisionManager))
    {
        printf("Error: Could not initialize enemy.\n");
        delete enemy;
        delete collider;
        for (auto& attack : attacks)
            delete attack;
        for (auto& [type, animation] : *descriptor.animations)
            delete animation;
        delete descriptor.animations;
        return nullptr;
    }

    return enemy;
}

std::unordered_map<AnimationType, Animation*> EnemyFactory::loadAnimations(const json& config)
{
    return AnimationLoader::LoadAnimations(config["Animations"]);
}

std::vector<Attack*> EnemyFactory::loadAttacks(const json& config)
{
    std::vector<Attack*> attacks;

    if (config.contains("Attacks"))
    {
        for (const auto& [attackName, attackData] : config["Attacks"].items())
        {
            float damage = attackData["Damage"].get<float>();
            float lifetime = attackData["Lifetime"].get<float>();

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
                attacks.push_back(new MeleeAttack(damage, lifetime, attackCollider));
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

                attacks.push_back(new RangedAttack(damage, animation, lifetime, attackData["Speed"].get<float>(), attackData["FireRate"].get<float>(), attackCollider));
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
