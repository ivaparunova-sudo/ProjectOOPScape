#pragma once
#include "Entity.h"
#include "Point.h"
#include <queue>
#include <vector>
#include <string>

class Board;

class Enemy : public Entity
{
public:
	Enemy(int x, int y, int health, Power power, char symbol);
	virtual ~Enemy() = default;

	char getSymbol() const override;
	virtual std::string getTypeName() const;

	bool tryMoveTowardsHeroBFS(Point& enemyPos, const Point& heroPos, const Board& board) const;
	virtual void takeTurn(const Point& heroPos, const Board& board);

	virtual int tryRangedAttack(const Point& heroPos, const Board& board) const;

	void takeDamage(int amount);
	bool isAlive() const;

	void stun();
	bool isStunned() const;
	void clearStun();

private:
	bool stunned = false;
};


class FastEnemy : public Enemy {
public:
	FastEnemy(int x, int y);

	char getSymbol() const override;
	std::string getTypeName() const override;

	void takeTurn(const Point& heroPos, const Board& board) override;
};


class BruteEnemy : public Enemy {
public:
	BruteEnemy(int x, int y);

	char getSymbol() const override;
	std::string getTypeName() const override;

	int tryRangedAttack(const Point& heroPos, const Board& board) const override;
};

class RangedEnemy : public Enemy {
	int range;
public:
	RangedEnemy(int x, int y);

	char getSymbol() const override;
	std::string getTypeName() const override;

	void takeTurn(const Point& heroPos, const Board& board) override;
	int tryRangedAttack(const Point& heroPos, const Board& board) const override;
};