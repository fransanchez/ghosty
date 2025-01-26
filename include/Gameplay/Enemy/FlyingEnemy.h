#pragma once

#include <Gameplay/Enemy/Enemy.h>

class FlyingEnemy : public Enemy
{
    public:
        FlyingEnemy() = default;
        virtual ~FlyingEnemy() override = default;

        void FlyingEnemy::update(float deltaMilliseconds) override;
    private:
        void handleIdleState() override;
        void handlePatrolState() override;
        void handleChaseState() override;
        void handleTargetLockedState() override;
        void handleReturnToOriginState() override;
        void handleAttackState() override;

        virtual bool isPlayerInRange() = 0;

        void applyOscillation();

        float m_oscillationTimer = 0;
};