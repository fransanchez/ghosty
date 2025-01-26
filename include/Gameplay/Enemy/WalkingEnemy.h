#pragma once

#include <Gameplay/Enemy/Enemy.h>

class WalkingEnemy : public Enemy
{
    public:
        WalkingEnemy() = default;
        virtual ~WalkingEnemy() override = default;

        virtual void update(float deltaMilliseconds) override; // From GameObject
    private:
        void handleIdleState() override;
        void handlePatrolState() override;
        void handleChaseState() override;
        void handleTargetLockedState() override;
        void handleReturnToOriginState() override;
        void handleAttackState() override;
        virtual bool isPlayerInRange() = 0;

        float m_stateTimer{ 0.0f }; // Time spent in the current state
        const float IDLE_DURATION = 2000.0f; // Time to stay idle before patrolling (ms)
        const float PATROL_DURATION = 5000.0f;
};