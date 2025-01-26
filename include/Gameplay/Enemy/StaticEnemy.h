#pragma once

#include <Gameplay/Enemy/Enemy.h>

class StaticEnemy : public Enemy
{
    public:
        StaticEnemy() = default;
        virtual ~StaticEnemy() override = default;

        virtual void update(float deltaMilliseconds) override; // From GameObject
    private:
        void handleIdleState() override;
        void handlePatrolState() override;
        void handleChaseState() override;
        void handleTargetLockedState() override;
        void handleReturnToOriginState() override;
        void handleAttackState() override;
        virtual bool isPlayerInRange() = 0;

        float m_watchTimer{ 0.0f };
};