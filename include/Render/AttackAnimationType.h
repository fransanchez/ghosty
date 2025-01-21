#pragma once
#include <string>

enum class AttackAnimationType
{
    None,
    FireProjectile
};

inline AttackAnimationType parseAttackAnimationType(const std::string& key)
{
    if (key == "FireProjectile") return AttackAnimationType::FireProjectile;
    return AttackAnimationType::None;
}