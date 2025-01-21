#include <Render/AnimationLoader.h>
#include <Core/AssetManager.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

std::unordered_map<AnimationType, Animation> AnimationLoader::LoadPlayerAnimations(const std::string& configFile)
{
    std::ifstream file(configFile);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open configuration file: " + configFile);
    }

    json config;
    file >> config;

    std::unordered_map<AnimationType, Animation> animations;

    for (const auto& [key, animationData] : config["Animations"].items())
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

std::unordered_map<AttackAnimationType, AttackAnimation> AnimationLoader::LoadAttackAnimations(const std::string& configFile)
{
    std::ifstream file(configFile);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open configuration file: " + configFile);
    }

    json config;
    file >> config;

    std::unordered_map<AttackAnimationType, AttackAnimation> attackAnimations;

    if (config.contains("Attacks"))
    {
        for (const auto& [attackName, attackData] : config["Attacks"].items())
        {
            Animation baseAnimation = LoadAnimation(
                attackData["TexturePath"].get<std::string>(),
                attackData["FrameCountRows"].get<int>(),
                attackData["FrameCountColumns"].get<int>(),
                attackData.contains("FrameDuration") ? attackData["FrameDuration"].get<float>() : 0.1f,
                attackData.contains("Loop") ? attackData["Loop"].get<bool>() : true
            );

            float effectTriggerTime = attackData.contains("EffectTriggerTime")
                ? attackData["EffectTriggerTime"].get<float>()
                : 0.0f;

            AttackAnimation attackAnimation(baseAnimation, effectTriggerTime);
            AttackAnimationType type = parseAttackAnimationType(attackName);
            attackAnimations[type] = std::move(attackAnimation);
        }
    }

    return attackAnimations;
}

Animation AnimationLoader::LoadAnimation(const std::string& texturePath, int frameRows, int frameColumns, float frameDuration, bool loop)
{
    sf::Texture* texture = AssetManager::getInstance()->loadTexture(texturePath.c_str());
    if (!texture)
    {
        throw std::runtime_error("Failed to load texture: " + texturePath);
    }

    int frameWidth = texture->getSize().x / frameColumns;
    int frameHeight = texture->getSize().y / frameRows;

    Animation animation;
    for (int row = 0; row < frameRows; ++row)
    {
        for (int col = 0; col < frameColumns; ++col)
        {
            sf::Texture* frameTexture = new sf::Texture();
            if (frameTexture->loadFromImage(
                texture->copyToImage(),
                sf::IntRect(col * frameWidth, row * frameHeight, frameWidth, frameHeight)))
            {
                animation.addFrame(frameTexture);
            }
            else
            {
                delete frameTexture;
                printf("Error: Could not extract frame (%d, %d) from %s\n", row, col, texturePath.c_str());
            }
        }
    }

    animation.setFrameDuration(frameDuration);
    animation.setLoop(loop);
    return animation;
}
