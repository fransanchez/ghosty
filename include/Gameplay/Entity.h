#pragma once

#include <Gameplay/Collisions/Collisionable.h>
#include <Gameplay/EntityLife.h>
#include <Render/Animation.h>
#include <Render/AnimationType.h>

class Attack;

class Entity : public Collisionable
{
    public:
        Entity() = default;
        virtual ~Entity() override;

        bool initEntity(std::unordered_map<AnimationType, Animation*>* animations,
            std::vector<Attack*> attacks,
            int maxLife,
            const sf::Vector2f& position);

        int getCurrentLives();
        int getMaxLives();
        void reduceLives(int damage);
        bool isMarkedForDestruction() const { return m_markedForDestruction; };

        virtual void update(float deltaMilliseconds) override;
        virtual void render(sf::RenderWindow& window) override;

    protected:
        void updateAnimationSprite(float deltaMilliseconds);
        void updateAttacks(float deltaMilliseconds);
        bool shouldEndInvincibility();
        void setAnimationType(AnimationType desiredAnimationType);

        bool isInvincible() const{ return m_isInvincible; };
        void setInvincibility(bool newValue) { m_isInvincible = newValue; };
        bool isDead() const { return m_isDead; };
        void setIsDead(bool newValue) { m_isDead = newValue; };
        bool isAttacking() const { return m_isAttacking; };
        void setIsAttacking(bool newValue) { m_isAttacking = newValue; };
        bool isMovingRight() const { return m_movingRight; };
        void setMovingRight(bool newValue) { m_movingRight = newValue; };


        std::vector<Attack*> m_attacks;
        int m_currentAttackIndex{ 0 };

        sf::Sprite m_sprite;
        std::unordered_map<AnimationType, Animation*>* m_animations{ nullptr };
        Animation* m_currentAnimation{ nullptr };

        EntityLife m_life;
        bool m_isDead{ false };
        bool m_markedForDestruction{ false };
        bool m_isInvincible{ false };
        bool m_isAttacking{ false };
        bool m_movingRight{ false };
};