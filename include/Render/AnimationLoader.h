#pragma once

#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <Render/AttackAnimation.h>
#include <Render/AttackAnimationType.h>
#include <unordered_map>
#include <string>

class AnimationLoader
{
public:
    static std::unordered_map<AnimationType, Animation> LoadPlayerAnimations(const std::string& configFile);

    static std::unordered_map<AttackAnimationType, AttackAnimation> LoadAttackAnimations(const std::string& configFile);

private:
    static Animation LoadAnimation(const std::string& texturePath, int frameRows, int frameColumns, float frameDuration, bool loop);

};
