#pragma once

#include <Gameplay/Enemy/WalkingEnemy.h>

class SkeletonEnemy : public WalkingEnemy
{
    public:
        SkeletonEnemy() = default;
        ~SkeletonEnemy() override = default;

    private:
        bool SkeletonEnemy::isPlayerInRange() override;
        SoundType getHurtSoundType() override;
};