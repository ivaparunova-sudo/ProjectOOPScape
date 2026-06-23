#include "Heroes.h"
#include "Board.h"
#include <climits>
#include <cmath>

// ---------------- Wizard ----------------

Wizard::Wizard()
    : Player(0, 0, 100, 1, Power("Teleport", 0), 'H', 5) {
}

Wizard::Wizard(int x, int y, int health, int speed)
    : Player(x, y, health, speed, Power("Teleport", 0), 'H', 5) {
}

std::string Wizard::getClassName() const { return "Wizard"; }
std::string Wizard::getAbilityName() const { return "Teleport"; }

bool Wizard::useOOP(const Board& board, const std::vector<Enemy*>& enemies) {
    if (!canUseOOP()) return false;

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
            if (minEnemyDist == INT_MAX) minEnemyDist = 0;

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

// ---------------- Knight ----------------

Knight::Knight()
    : Player(0, 0, 120, 1, Power("Power Strike", 30), 'H', 4), strikeDamage(30) {
}

Knight::Knight(int x, int y, int health, int speed)
    : Player(x, y, health, speed, Power("Power Strike", 30), 'H', 4), strikeDamage(30) {
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
            e->takeDamage(strikeDamage);
            hitAny = true;
        }
    }

    oopCooldown = oopMaxCooldown;
    return hitAny;
}

// ---------------- Healer ----------------

Healer::Healer()
    : Player(0, 0, 90, 1, Power("Heal", 0), 'H', 6), healAmount(40), invulnTurns(2) {
}

Healer::Healer(int x, int y, int health, int speed)
    : Player(x, y, health, speed, Power("Heal", 0), 'H', 6), healAmount(40), invulnTurns(2) {
}

std::string Healer::getClassName() const { return "Healer"; }
std::string Healer::getAbilityName() const { return "Heal"; }

bool Healer::useOOP(const Board& board, const std::vector<Enemy*>& enemies) {
    (void)board;
    (void)enemies;
    if (!canUseOOP()) return false;

    heal(healAmount);
    grantInvulnerability(invulnTurns);
    oopCooldown = oopMaxCooldown;
    return true;
}