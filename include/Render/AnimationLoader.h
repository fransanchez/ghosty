#pragma once

#include <nlohmann/json.hpp>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <unordered_map>
#include <string>


class AnimationLoader
{
public:
    static std::unordered_map<AnimationType, Animation> LoadPlayerAnimations(const std::string& configFile);
    static Animation LoadSingleAttackAnimation(const nlohmann::json& animationData);

private:
    static Animation LoadAnimation(const std::string& texturePath, int frameRows, int frameColumns, float frameDuration, bool loop);

};
