#pragma once
#include "Entity.h"
#include "Point.h"
#include <queue>
#include <vector>
#include <string>

class Board;

// Base enemy: chases the hero using BFS pathfinding and moves exactly
// one step per turn. Catching the hero (sharing its cell) ends the game
// immediately, per the assignment's loss condition.
class Enemy : public Entity
{
public:
	Enemy();
	Enemy(int x, int y, int health, int speed, Power power, char symbol);
	Enemy(const Enemy& other);
	virtual ~Enemy() = default;

	char getSymbol() const override;
	virtual std::string getTypeName() const;

	bool tryMoveTowardsHeroBFS(Point& enemyPos, const Point& heroPos, const Board& board) const;
	virtual void takeTurn(const Point& heroPos, const Board& board);

	// Ranged enemies override this to damage the hero from a distance
	// without needing to occupy the hero's cell. Returns the damage dealt
	// (0 if no attack happened, e.g. out of range or no line of sight).
	virtual int tryRangedAttack(const Point& heroPos, const Board& board) const;

	void takeDamage(int amount);
	bool isAlive() const;
};

// Moves two steps per turn instead of one.
class FastEnemy : public Enemy {
public:
	FastEnemy();
	FastEnemy(int x, int y);

	char getSymbol() const override;
	std::string getTypeName() const override;

	void takeTurn(const Point& heroPos, const Board& board) override;
};

// Tougher melee enemy: higher health and resists the Knight's Power
// Strike better than a regular Enemy, but still moves one step per turn.
class BruteEnemy : public Enemy {
public:
	BruteEnemy();
	BruteEnemy(int x, int y);

	char getSymbol() const override;
	std::string getTypeName() const override;
};

// Stationary archer: does not chase the hero. Instead, every turn it
// checks whether the hero is in the same row/column within range and
// with a clear line of sight (no walls in between); if so, it deals
// damage to the hero from a distance instead of moving.
class RangedEnemy : public Enemy {
	int range;
	int rangedDamage;
public:
	RangedEnemy();
	RangedEnemy(int x, int y);

	char getSymbol() const override;
	std::string getTypeName() const override;

	void takeTurn(const Point& heroPos, const Board& board) override;
	int tryRangedAttack(const Point& heroPos, const Board& board) const override;
};