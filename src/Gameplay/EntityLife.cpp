#include <Gameplay/EntityLife.h>
#include <algorithm>

void EntityLife::addLife(int amount)
{
    if (amount <= 0) return;
    m_currentLife = std::min(m_currentLife + amount, m_maxLife);
}

void EntityLife::subtractLife(int amount)
{
    if (amount <= 0) return;
    m_currentLife = std::max(m_currentLife - amount, 0);
}

int EntityLife::getLife() const
{
    return m_currentLife;
}

int EntityLife::getMaxLife() const
{
    return m_maxLife;
}
