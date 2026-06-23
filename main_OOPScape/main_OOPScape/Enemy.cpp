#include "Enemy.h"
#include "Board.h"

Enemy::Enemy()
    : Entity(0, 0, 50, 1, Power(), 'E') {
}

Enemy::Enemy(int x, int y, int health, int speed, Power power, char symbol)
    : Entity(x, y, health, speed, power, symbol) {
}

Enemy::Enemy(const Enemy& other)
    : Entity(other) {
}

char Enemy::getSymbol() const { return symbol; }

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



FastEnemy::FastEnemy()
    : Enemy(0, 0, 30, 2, Power(), 'F') {
}

FastEnemy::FastEnemy(int x, int y)
    : Enemy(x, y, 30, 2, Power(), 'F') {
}

char FastEnemy::getSymbol() const { return 'F'; }

void FastEnemy::takeTurn(const Point& heroPos, const Board& board) {

    for (int i = 0; i < 2; ++i) {
        Point pos(x, y);
        if (tryMoveTowardsHeroBFS(pos, heroPos, board)) {
            x = pos.getX();
            y = pos.getY();
        }
    }
}