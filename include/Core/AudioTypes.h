#pragma once

#include <iostream>
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

enum class MusicType
{
    MainMenu,
    Game
};

enum class SoundType
{
    ButtonClick,
    DarkBall,
    DinoAttack,
    DinoHurt,
    FireBall,
    GameOver,
    GhostAttack,
    GhostHurt,
    IceSpike,
    ItemPickup,
    Jump,
    PlayerDeath,
    PlayerHurt,
    SkeletonAttack,
    SkeletonHurt,
    VampireAttack,
    VampireHurt,
    Victory,
    WindMagic
};

inline SoundType parseSoundType(const std::string& type) {
    static const std::unordered_map<std::string, SoundType> soundMap = {
        {"ButtonClick", SoundType::ButtonClick},
        {"DarkBall", SoundType::DarkBall},
        {"DinoAttack", SoundType::DinoAttack},
        {"DinoHurt", SoundType::DinoHurt},
        {"FireBall", SoundType::FireBall},
        {"GameOver", SoundType::GameOver},
        {"GhostAttack", SoundType::GhostAttack},
        {"GhostHurt", SoundType::GhostHurt},
        {"IceSpike", SoundType::IceSpike},
        {"ItemPickup", SoundType::ItemPickup},
        {"Jump", SoundType::Jump},
        {"PlayerDeath", SoundType::PlayerDeath},
        {"PlayerHurt", SoundType::PlayerHurt},
        {"SkeletonAttack", SoundType::SkeletonAttack},
        {"SkeletonHurt", SoundType::SkeletonHurt},
        {"VampireAttack", SoundType::VampireAttack},
        {"VampireHurt", SoundType::VampireHurt},
        {"Victory", SoundType::Victory},
        {"WindMagic", SoundType::WindMagic}

    };

    auto it = soundMap.find(type);
    if (it != soundMap.end()) {
        return it->second;
    }
    else {
        std::cerr << "Error: SoundType not found for " << type << std::endl;
        return SoundType::ButtonClick;
    }
}

inline MusicType parseMusicType(const std::string& type) {
    static const std::unordered_map<std::string, MusicType> musicMap = {
        {"MainMenu", MusicType::MainMenu},
        {"Game", MusicType::Game}
    };

    auto it = musicMap.find(type);
    if (it != musicMap.end()) {
        return it->second;
    }
    else {
        std::cerr << "Error: MusicType not found for " << type << std::endl;
        return MusicType::MainMenu;
    }
}