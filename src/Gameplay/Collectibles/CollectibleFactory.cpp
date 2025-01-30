#include <Gameplay/Collectibles/CollectibleFactory.h>
#include <Render/AnimationLoader.h>
#include <Core/AssetManager.h>
#include <Utils/Constants.h>
#include <fstream>

using json = nlohmann::json;

std::unordered_map<CollectibleType, Animation*> CollectibleFactory::loadCollectibleAnimations()
{
    std::unordered_map<CollectibleType, Animation*> collectibleAnimations;

    std::ifstream file(COLLECTIBLES_CONFIG_PATH);
    if (!file.is_open())
    {
        printf("Error: Could not open collectibles configuration file %s\n", COLLECTIBLES_CONFIG_PATH);
        return collectibleAnimations;
    }

    json config;
    file >> config;

    for (const auto& item : config["Collectibles"])
    {
        CollectibleType type;
        std::string typeString = item["Type"];

        Animation* animation = AnimationLoader::LoadSingleAnimationFromJson(item["Animation"]);
        if (animation)
        {
            type = parseCollectibleType(typeString);
            collectibleAnimations[type] = animation;
        }
    }

    return collectibleAnimations;
}
