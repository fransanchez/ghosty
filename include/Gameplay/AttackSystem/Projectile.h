#pragma once

#include <Gameplay/Collisions/Collisionable.h>
#include <Gameplay/GameObject.h>
#include <SFML/Graphics.hpp>
#include <Render/Animation.h>
#include <cstdio>

class Projectile : public Collisionable
{
    public:
        struct ProjectileDescriptor
        {
            sf::Vector2f position;
            sf::Vector2f direction{ .0f, .0f };
            float projectileSpeed;
            float projectileLife;
            int damage = 0;
        };

        Projectile() : m_animation(nullptr) {};

        ~Projectile() override;

        void init(ProjectileDescriptor descriptor,
            const Animation* anim,
            Collider* collider,
            CollisionManager* collisionManager);

        sf::Vector2f getPosition() const;
        const Animation* getAnimation() const;

        void update(float deltaMilliseconds) override; // From GameObject
        void render(sf::RenderWindow& window) override; // From GameObject
        void handleCollisions() override; // From Collisionable

        bool isExpired() const;
        void markForDestruction();
        bool isMarkedForDestruction() const;
        int getDamage() const;
        void unload();

    private:
        int m_damage = 0;
        sf::Sprite m_sprite;
        sf::Vector2f m_direction;
        bool m_markedForDestruction{ false };
        float m_lifetime{ 0.0f };
        float m_projectileSpeed{ 0.0f };
        // We want a copy of the animation, not a pointer to the base one
        Animation* m_animation;
};
