#pragma once
#include "Entity.h"
#include "Point.h"
#include <queue>
#include <vector>

class Board;

class Enemy : public Entity
{
public:
	Enemy();
	Enemy(int x, int y, int health, int speed, Power power, char symbol);
	Enemy(const Enemy& other);
	virtual ~Enemy() = default;

	char getSymbol() const override;

	bool tryMoveTowardsHeroBFS(Point& enemyPos, const Point& heroPos, const Board& board) const;
	virtual void takeTurn(const Point& heroPos, const Board& board);
};

class FastEnemy : public Enemy {
public:
	FastEnemy();
	FastEnemy(int x, int y);

	char getSymbol() const override;

	void takeTurn(const Point& heroPos, const Board& board) override;
};


