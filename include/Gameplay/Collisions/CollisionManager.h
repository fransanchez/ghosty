#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

class Attack;
enum class AttackFaction;
class Collectible;
class Collider;
class Enemy;
class Player;
class Projectile;

namespace sf
{
    class Shape;
}

struct WallCollision
{
    bool collided = false;
    bool leftCollision = false;
    bool rightCollision = false;
    bool topCollision = false;
};

struct PatrolAreaCollision
{
    bool inside = false;
    bool leftEdge = false;
    bool rightEdge = false;
    sf::FloatRect areaBounds;
};

class CollisionManager
{
    public:
        ~CollisionManager();

        void registerEnemy(Enemy* enemy);
        void unregisterEnemy(Enemy* enemy);
        void registerPlayer(Player* player);
        void unregisterPlayer();
        void registerProjectile(Projectile* projectile, AttackFaction faction);
        void unregisterProjectile(Projectile* projectile, AttackFaction faction);
        void registerMeleeAttack(Attack* attack);
        void unregisterMeleeAttack(Attack* attack);
        void registerCollectible(Collectible* collectible);
        void unregisterCollectible(Collectible* collectible);
        Collectible* checkPlayerCollectibleCollision();

        void setGroundShapes(const std::vector<sf::Shape*>& groundShapes);
        void setWallShapes(const std::vector<sf::Shape*>& wallShapes);
        void setEnemyPatrolAreasShapes(const std::vector<sf::Shape*>& patrolAreasShapes);

        bool checkIsGrounded(const Collider* collider) const;
        WallCollision checkWalls(const Collider* collider) const;
        PatrolAreaCollision checkPatrolArea(const Collider* collider, const sf::Shape* patrolArea) const;
        bool isPlayerInsideArea(const sf::FloatRect& area) const;

        const sf::Shape* getClosestPatrolArea(const sf::Vector2f& spawnPoint) const;

        sf::Vector2f getPlayerPosition() const;

        int checkPlayerHurtingCollisions();
        int checkEnemyHurtingCollisions(const Collider* collider);

    private:
        const float GROUND_COLLISION_MARGIN = 6.f;

        std::vector<sf::Shape*> m_groundShapes;
        std::vector<sf::Shape*> m_wallShapes;
        std::vector<sf::Shape*> m_enemyPatrolAreasShapes;

        Player* m_player = nullptr;
        std::vector<Projectile*> m_playerProjectiles;
        std::vector<Enemy*> m_enemies;
        std::vector<Projectile*> m_enemyProjectiles;
        std::vector<Attack*> m_enemyMeleeAttacks;
        std::vector<Collectible*> m_collectibles;
};
