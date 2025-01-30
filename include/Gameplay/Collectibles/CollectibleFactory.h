#pragma once

#include <Gameplay/Collectibles/CollectibleType.h>
#include <unordered_map>

class Animation;

class CollectibleFactory
{
public:
    static std::unordered_map<CollectibleType, Animation*> loadCollectibleAnimations();
};
