#include <Core/AssetManager.h>
#include <fstream>
#include <Gameplay/Collisions/Collider.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/DinoEnemy.h>
#include <Gameplay/Enemy/Enemy.h>
#include <Gameplay/Enemy/EnemyFactory.h>
#include <Gameplay/Enemy/EnemyType.h>
#include <Gameplay/Enemy/GhostEnemy.h>
#include <Gameplay/Enemy/SkeletonEnemy.h>
#include <Gameplay/Enemy/VampireEnemy.h>
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

Enemy* EnemyFactory::createEnemy(const EnemyType enemyType, const sf::Vector2f& position, CollisionManager* collisionManager)
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
        return nullptr;
    }

    const std::string& configPath = it->second;

    std::ifstream file(configPath);
    if (!file.is_open())
    {
        printf("Error: Could not open enemy configuration file %s\n", configPath.c_str());
        return nullptr;
    }

    json config;
    file >> config;

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

    sf::Vector2f sightArea = { 300.f, 150.f }; // Default sight area
    if (config.contains("SightArea"))
    {
        sightArea.x = config["SightArea"]["Width"].get<float>();
        sightArea.y = config["SightArea"]["Height"].get<float>();
    }

    // Create enemy based on type
    Enemy* enemy = nullptr;
    switch (enemyType)
    {
    case EnemyType::Ghost:
        enemy = new GhostEnemy();
        break;
    case EnemyType::Skeleton:
        enemy = new SkeletonEnemy();
        break;
    case EnemyType::Dino:
        enemy = new DinoEnemy();
        break;
    case EnemyType::Vampire:
        enemy = new VampireEnemy();
        break;
    default:
        printf("Error: Unsupported EnemyType\n");
        delete collider;
        return nullptr;
    }

    const sf::Shape* patrolArea = collisionManager->getClosestPatrolArea(position);
    if (!patrolArea)
    {
        printf("Error: No patrol area found for enemy at position (%f, %f).\n", position.x, position.y);
        return nullptr;
    }

    int maxLife = config.contains("MaxLife") ? config["MaxLife"].get<int>() : 1;

    Enemy::EnemyDescriptor descriptor;
    descriptor.position = position;
    descriptor.speed = speed;
    descriptor.animations = new std::unordered_map<AnimationType, Animation*>(std::move(animations));
    descriptor.attacks = attacks;
    descriptor.sightArea = sightArea;
    descriptor.patrolArea = patrolArea;
    descriptor.maxLife = maxLife;

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
                float attackRate = attackData.contains("AttackRate") ? attackData["AttackRate"].get<float>() : 1.0f;
                attacks.push_back(new MeleeAttack(AttackFaction::Enemy, damage, lifetime, attackRate, attackCollider));
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
                float fireRate = attackData["FireRate"].get<float>();
                float range = attackData["Range"].get<float>();

                attacks.push_back(new RangedAttack(
                    AttackFaction::Enemy,
                    damage,
                    animation,
                    lifetime,
                    speed,
                    fireRate,
                    range,
                    attackCollider));
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
