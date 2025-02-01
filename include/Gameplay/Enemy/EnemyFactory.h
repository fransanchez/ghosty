#pragma once

#include <nlohmann/json.hpp>
#include <Render/AnimationType.h>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <Gameplay/Enemy/EnemyType.h>


class Enemy;
class Collider;
class CollisionManager;
class Animation;
class Attack;

class EnemyFactory
{
public:
    static Enemy::EnemyDescriptor* EnemyFactory::loadEnemyDescriptor(const EnemyType enemyType, CollisionManager* collisionManager);
private:
    static std::unordered_map<AnimationType, Animation*> loadAnimations(const nlohmann::json& config);
    static std::vector<Attack*> loadAttacks(const nlohmann::json& config, CollisionManager* collisionManager);
    static Collider* loadCollider(const nlohmann::json& parentData, const sf::Vector2f& position);
    static void loadEnemySounds(const nlohmann::json& config);
};
