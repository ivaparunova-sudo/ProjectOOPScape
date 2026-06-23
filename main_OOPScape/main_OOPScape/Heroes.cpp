#include "Heroes.h"
#include "Board.h"
#include <climits>
#include <cmath>



Wizard::Wizard(int x, int y, int health)
    : Player(x, y, health, Power("Teleport", 0), 'H', 5) {
}

std::string Wizard::getClassName() const { return "Wizard"; }
std::string Wizard::getAbilityName() const { return "Teleport"; }

bool Wizard::useOOP(const Board& board, const std::vector<Enemy*>& enemies) {
    if (!canUseOOP()) return false;

    bool anyEnemyAlive = false;
    for (const Enemy* e : enemies) {
        if (e->isAlive()) { anyEnemyAlive = true; break; }
    }
    if (!anyEnemyAlive) return false;

    int n = board.getSize();
    int bestDist = -1;
    int bestX = x, bestY = y;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (!board.isWalkable(col, row)) continue;

            int minEnemyDist = INT_MAX;
            for (const Enemy* e : enemies) {
                if (!e->isAlive()) continue;
                int dist = std::abs(e->getX() - col) + std::abs(e->getY() - row);
                if (dist < minEnemyDist) minEnemyDist = dist;
            }

            if (minEnemyDist > bestDist) {
                bestDist = minEnemyDist;
                bestX = col;
                bestY = row;
            }
        }
    }

    x = bestX;
    y = bestY;
    oopCooldown = oopMaxCooldown;
    return true;
}


Knight::Knight(int x, int y, int health)
    : Player(x, y, health, Power("Power Strike", 30), 'H', 4) {
}

std::string Knight::getClassName() const { return "Knight"; }
std::string Knight::getAbilityName() const { return "Power Strike"; }

bool Knight::useOOP(const Board& board, const std::vector<Enemy*>& enemies) {
    (void)board;
    if (!canUseOOP()) return false;

    bool hitAny = false;
    for (Enemy* e : enemies) {
        if (!e->isAlive()) continue;
        int dx = std::abs(e->getX() - x);
        int dy = std::abs(e->getY() - y);
        if (dx <= 1 && dy <= 1 && (dx + dy) > 0) {
            e->takeDamage(getPower().getDamage());
            if (e->isAlive()) e->stun(); 
            hitAny = true;
        }
    }

    oopCooldown = oopMaxCooldown;
    return hitAny;
}



Healer::Healer(int x, int y, int health)
    : Player(x, y, health, Power("Heal", 40), 'H', 6), invulnTurns(2) {
}

std::string Healer::getClassName() const { return "Healer"; }
std::string Healer::getAbilityName() const { return "Heal"; }

bool Healer::useOOP(const Board& board, const std::vector<Enemy*>& enemies) {
    (void)board;
    (void)enemies;
    if (!canUseOOP()) return false;

    heal(getPower().getDamage());
    grantInvulnerability(invulnTurns);
    oopCooldown = oopMaxCooldown;
    return true;
}