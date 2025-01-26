#pragma once

#include <string>

enum class EnemyType
{
    Ghost,
    Skeleton,
    Unknown
};

inline EnemyType stringToEnemyType(const std::string& typeString)
{
    if (typeString == "Ghost") return EnemyType::Ghost;
    if (typeString == "Skeleton") return EnemyType::Skeleton;
    return EnemyType::Unknown;
}