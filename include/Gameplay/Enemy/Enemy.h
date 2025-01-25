#pragma once

#include <Gameplay/Collisionable.h>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Attack;
class Collider;

class Enemy : public Collisionable
{
	public:

		struct EnemyDescriptor
		{
			sf::Vector2f position;
			sf::Vector2f speed{ .0f, .0f };
			std::unordered_map<AnimationType, Animation*>* animations;
			std::vector<Attack*> attacks;
		};

		~Enemy() override = default;

		bool init(const EnemyDescriptor& enemyDescriptor, Collider* collider, CollisionManager* collisionManager);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void update(float deltaMilliseconds) override; // From GameObject
		void render(sf::RenderWindow& window) override; // From GameObject
		void handleCollisions() override; // From Collisionable

	protected:

		sf::Sprite m_sprite;
		sf::Vector2f m_direction{ .0f, .0f };
		sf::Vector2f m_speed{ 0.f, 0.f };
		std::unordered_map<AnimationType, Animation*>* m_animations;
		std::vector<Attack*> m_attacks;

		Animation* m_currentAnimation{ nullptr };
		int m_currentAttackIndex{ 0 };
};