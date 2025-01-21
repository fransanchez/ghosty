#pragma once

#include <Render/Animation.h>

class AttackAnimation : public Animation
{
public:
   
    AttackAnimation() = default;

    AttackAnimation(const Animation& baseAnimation, float effectTriggerTime = 0.0f)
        : Animation(baseAnimation), m_effectTriggerTime(effectTriggerTime)
    {
    }

    void setEffectTriggerTime(float triggerTime) { m_effectTriggerTime = triggerTime; }
    float getEffectTriggerTime() const { return m_effectTriggerTime; }

private:
    float m_effectTriggerTime{ 0.0f };
};
