#pragma once

class EntityLife
{
public:
    EntityLife(int maxLife) : m_maxLife(maxLife), m_currentLife(maxLife) {}

    EntityLife() = default;


    void addLife(int amount);
    void subtractLife(int amount);

    int getLife() const;
    int getMaxLife() const;

private:
    int m_currentLife;
    int m_maxLife;
};
