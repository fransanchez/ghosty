#pragma once

#include <string>

enum class EnemyType
{
    Ghost,
    Skeleton,
    Dino,
    Unknown
};

inline EnemyType stringToEnemyType(const std::string& typeString)
{
    if (typeString == "Ghost") return EnemyType::Ghost;
    if (typeString == "Skeleton") return EnemyType::Skeleton;
    if (typeString == "Dino") return EnemyType::Dino;
    return EnemyType::Unknown;
}