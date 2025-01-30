#include <Render/AnimationLoader.h>
#include <Core/AssetManager.h>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

std::unordered_map<AnimationType, Animation*> AnimationLoader::LoadPlayerAnimations(const json& config)
{
    return LoadAnimations(config["Animations"]);
}

std::unordered_map<AnimationType, Animation*> AnimationLoader::LoadAnimations(const json& animationsJson)
{
    std::unordered_map<AnimationType, Animation*> animations;

    for (const auto& [key, animationData] : animationsJson.items())
    {
        AnimationType type = parseAnimationType(key);
        animations[type] = LoadAnimation(
            animationData["TexturePath"].get<std::string>(),
            animationData["FrameCountRows"].get<int>(),
            animationData["FrameCountColumns"].get<int>(),
            animationData.contains("FrameDuration") ? animationData["FrameDuration"].get<float>() : 0.1f,
            animationData.contains("Loop") ? animationData["Loop"].get<bool>() : true
        );
    }

    return animations;
}

Animation* AnimationLoader::LoadSingleAnimationFromJson(const json& animationData)
{
    return LoadAnimation(
        animationData["TexturePath"].get<std::string>(),
        animationData["FrameCountRows"].get<int>(),
        animationData["FrameCountColumns"].get<int>(),
        animationData.contains("FrameDuration") ? animationData["FrameDuration"].get<float>() : 0.1f,
        animationData.contains("Loop") ? animationData["Loop"].get<bool>() : true
    );
}

Animation* AnimationLoader::LoadAnimation(const std::string& texturePath, int frameRows, int frameColumns, float frameDuration, bool loop)
{
    sf::Texture* texture = AssetManager::getInstance()->loadTexture(texturePath.c_str());
    if (!texture)
    {
        throw std::runtime_error("Failed to load texture: " + texturePath);
    }

    int frameWidth = texture->getSize().x / frameColumns;
    int frameHeight = texture->getSize().y / frameRows;

    Animation* animation = new Animation();
    for (int row = 0; row < frameRows; ++row)
    {
        for (int col = 0; col < frameColumns; ++col)
        {
            sf::IntRect frameRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
            animation->addFrame(texture, frameRect);
        }
    }

    animation->setFrameDuration(frameDuration);
    animation->setLoop(loop);
    return animation;
}
