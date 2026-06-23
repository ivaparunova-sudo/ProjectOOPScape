#include "Enemy.h"
#include "Board.h"
#include <cmath>

Enemy::Enemy(int x, int y, int health, Power power, char symbol)
    : Entity(x, y, health, power, symbol) {
}

char Enemy::getSymbol() const { return symbol; }

std::string Enemy::getTypeName() const { return "Enemy"; }

bool Enemy::tryMoveTowardsHeroBFS(Point& enemyPos, const Point& heroPos, const Board& board) const {
    int n = board.getSize();
    int W = n;
    std::vector<int> parent(W * W, -1);
    std::queue<Point> q;

    int start = enemyPos.getY() * W + enemyPos.getX();
    int goal = heroPos.getY() * W + heroPos.getX();
    parent[start] = start;
    q.push(enemyPos);

    const int dx[4] = { -1, 1,  0, 0 };
    const int dy[4] = { 0, 0, -1, 1 };

    bool found = false;
    while (!q.empty()) {
        Point cur = q.front(); q.pop();
        int curI = cur.getY() * W + cur.getX();
        if (curI == goal) { found = true; break; }
        for (int k = 0; k < 4; ++k) {
            int nx = cur.getX() + dx[k];
            int ny = cur.getY() + dy[k];
            if (!board.isWalkable(nx, ny)) continue;
            int ni = ny * W + nx;
            if (parent[ni] != -1) continue;
            parent[ni] = curI;
            q.push(Point(nx, ny));
        }
    }

    if (!found) return false;

    int cur = goal;
    while (parent[cur] != start && cur != start)
        cur = parent[cur];
    if (cur == start) return false;

    enemyPos.setX(cur % W);
    enemyPos.setY(cur / W);
    return true;
}

void Enemy::takeTurn(const Point& heroPos, const Board& board) {
    Point pos(x, y);
    if (tryMoveTowardsHeroBFS(pos, heroPos, board)) {
        x = pos.getX();
        y = pos.getY();
    }
}

int Enemy::tryRangedAttack(const Point& heroPos, const Board& board) const {
    (void)heroPos;
    (void)board;
    return 0; // base enemies have no ranged attack
}

void Enemy::takeDamage(int amount) {
    if (amount <= 0) return;
    health -= amount;
    if (health < 0) health = 0;
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::stun() { stunned = true; }
bool Enemy::isStunned() const { return stunned; }
void Enemy::clearStun() { stunned = false; }



FastEnemy::FastEnemy(int x, int y)
    : Enemy(x, y, 30, Power("Bite", 0), 'V') {
}

char FastEnemy::getSymbol() const { return 'V'; }

std::string FastEnemy::getTypeName() const { return "Fast Enemy"; }

void FastEnemy::takeTurn(const Point& heroPos, const Board& board) {

    for (int i = 0; i < 2; ++i) {
        Point pos(x, y);
        if (tryMoveTowardsHeroBFS(pos, heroPos, board)) {
            x = pos.getX();
            y = pos.getY();
        }
    }
}



BruteEnemy::BruteEnemy(int x, int y)
    : Enemy(x, y, 120, Power("Smash", 25), 'B') {
}

char BruteEnemy::getSymbol() const { return 'B'; }

std::string BruteEnemy::getTypeName() const { return "Brute Enemy"; }

int BruteEnemy::tryRangedAttack(const Point& heroPos, const Board& board) const {
    (void)board; // melee range, no line-of-sight to check
    int dx = std::abs(heroPos.getX() - x);
    int dy = std::abs(heroPos.getY() - y);
    if (dx == 1 && dy == 1) return getPower().getDamage(); // diagonal only -- see class comment
    return 0;
}



RangedEnemy::RangedEnemy(int x, int y)
    : Enemy(x, y, 40, Power("Arrow", 10), 'R'), range(5) {
}

char RangedEnemy::getSymbol() const { return 'R'; }

std::string RangedEnemy::getTypeName() const { return "Ranged Enemy"; }

void RangedEnemy::takeTurn(const Point& heroPos, const Board& board) {
    // RangedEnemy is stationary: it never moves, it only fires (see
    // tryRangedAttack, invoked separately by Game). It intentionally
    // overrides takeTurn with a no-op to avoid chasing the hero.
    (void)heroPos;
    (void)board;
}

int RangedEnemy::tryRangedAttack(const Point& heroPos, const Board& board) const {
    bool sameRow = (heroPos.getY() == y);
    bool sameCol = (heroPos.getX() == x);
    if (!sameRow && !sameCol) return 0;

    int dist = sameRow ? std::abs(heroPos.getX() - x) : std::abs(heroPos.getY() - y);
    if (dist == 0 || dist > range) return 0;

    // Check line of sight: every cell strictly between the archer and the
    // hero must be walkable (no wall blocks the shot).
    int stepX = sameRow ? (heroPos.getX() > x ? 1 : -1) : 0;
    int stepY = sameCol ? (heroPos.getY() > y ? 1 : -1) : 0;

    int cx = x + stepX, cy = y + stepY;
    for (int i = 1; i < dist; ++i) {
        if (!board.isWalkable(cx, cy)) return 0;
        cx += stepX;
        cy += stepY;
    }

    return getPower().getDamage();
}