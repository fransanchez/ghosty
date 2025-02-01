#pragma once

#include <Gameplay/Enemy/StaticEnemy.h>

class VampireEnemy : public StaticEnemy
{
public:
    VampireEnemy() = default;
    ~VampireEnemy() override = default;

private:
    bool VampireEnemy::isPlayerInRange() override;
    SoundType getHurtSoundType() override;
};