#pragma once

#include <nlohmann/json.hpp>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <unordered_map>
#include <string>


class AnimationLoader
{
public:
    static std::unordered_map<AnimationType, Animation*> LoadPlayerAnimations(const nlohmann::json& configFile);
    static std::unordered_map<AnimationType, Animation*> LoadAnimations(const nlohmann::json& animationsJson);
    static Animation* LoadSingleAnimationFromJson(const nlohmann::json& animationData);

private:
    static Animation* LoadAnimation(const std::string& texturePath, int frameRows, int frameColumns, float frameDuration, bool loop);

};
