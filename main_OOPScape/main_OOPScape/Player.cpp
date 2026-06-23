#include "Player.h"
#include "Enemy.h"
#include <climits>
#include <cmath>

Player::Player()
    : Entity(0, 0, 100, 1, Power("Teleport", 0), 'H'), oopCooldown(0) {
}

Player::Player(int x, int y, int health, int speed, Power power, char symbol)
    : Entity(x, y, health, speed, power, symbol), oopCooldown(0) {
}

char Player::getSymbol() const { return symbol; }

bool Player::canUseOOP() const { return oopCooldown == 0; }
int  Player::getOopCooldown() const { return oopCooldown; }

void Player::setOopCooldown(int cooldown) { oopCooldown = cooldown; }

void Player::tickCooldown() {
    if (oopCooldown > 0) oopCooldown--;
}

bool Player::useOOP(const Board& board, const std::vector<Enemy>& enemies) {
    if (!canUseOOP()) return false;

    int n = board.getSize();
    int bestDist = -1;
    int bestX = x, bestY = y;

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (!board.isWalkable(col, row)) continue;

            int minEnemyDist = INT_MAX;
            for (const Enemy& e : enemies) {
                int dist = std::abs(e.getX() - col) + std::abs(e.getY() - row);
                if (dist < minEnemyDist) minEnemyDist = dist;
            }
            if (enemies.empty()) minEnemyDist = 0;

            if (minEnemyDist > bestDist) {
                bestDist = minEnemyDist;
                bestX = col;
                bestY = row;
            }
        }
    }

    x = bestX;
    y = bestY;
    oopCooldown = OOP_MAX_COOLDOWN;
    return true;
}