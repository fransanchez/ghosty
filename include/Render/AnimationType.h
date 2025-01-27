#pragma once

enum class AnimationType
{
    None,
    Idle,
    Walk,
    Run,
    Jump,
    Attack,
    WalkAttack,
    RunAttack,
    Hurt,
    Death
};

inline AnimationType parseAnimationType(const std::string& key)
{
    if (key == "Idle") return AnimationType::Idle;
    if (key == "Walk") return AnimationType::Walk;
    if (key == "Run") return AnimationType::Run;
    if (key == "Jump") return AnimationType::Jump;
    if (key == "Attack") return AnimationType::Attack;
    if (key == "WalkAttack") return AnimationType::WalkAttack;
    if (key == "RunAttack") return AnimationType::RunAttack;
    if (key == "Hurt") return AnimationType::Hurt;
    if (key == "Death") return AnimationType::Death;
    return AnimationType::None;
}