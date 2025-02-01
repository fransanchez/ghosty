#pragma once

#include <Gameplay/Enemy/StaticEnemy.h>

class DinoEnemy : public StaticEnemy
{
public:
    DinoEnemy() = default;
    ~DinoEnemy() override = default;

private:
    bool DinoEnemy::isPlayerInRange() override;
    SoundType getHurtSoundType() override;
};