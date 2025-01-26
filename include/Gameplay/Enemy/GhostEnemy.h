#pragma once

#include <Gameplay/Enemy/FlyingEnemy.h>

class GhostEnemy : public FlyingEnemy
{
    public:
        GhostEnemy() = default;
        ~GhostEnemy() override = default;

    private:
        bool isPlayerInRange() override;

};