#pragma once

#include <string>

enum class CollectibleType
{
    None,
    Life,
    Ice,
    Fire
};

inline CollectibleType parseCollectibleType(const std::string& key)
{
    if (key == "Life") return CollectibleType::Life;
    if (key == "Ice") return CollectibleType::Ice;
    if (key == "Fire") return CollectibleType::Fire;
    return CollectibleType::None;
}