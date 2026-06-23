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
	Enemy(int x, int y, int health, Power power, char symbol);
	virtual ~Enemy() = default;

	char getSymbol() const override;
	virtual std::string getTypeName() const;

	bool tryMoveTowardsHeroBFS(Point& enemyPos, const Point& heroPos, const Board& board) const;
	virtual void takeTurn(const Point& heroPos, const Board& board);

	// Ranged or diagonal-melee enemies override this to damage the hero
	// without needing to occupy the hero's cell (e.g. an archer firing
	// from range, or a Brute smashing a diagonally-adjacent hero it can't
	// yet step onto). Returns the damage dealt (0 if no attack happened).
	virtual int tryRangedAttack(const Point& heroPos, const Board& board) const;

	void takeDamage(int amount);
	bool isAlive() const;

	// Lets an ability (e.g. Knight's Power Strike) interrupt this enemy's
	// very next turn -- it skips both movement and ranged attack once,
	// then the stun clears itself. Without this, hitting an adjacent
	// enemy that survives the hit accomplishes nothing: the enemy still
	// gets to act immediately afterward and can still catch the hero on
	// the same turn the ability was used.
	void stun();
	bool isStunned() const;
	void clearStun();

private:
	bool stunned = false;
};

// Moves two steps per turn instead of one.
class FastEnemy : public Enemy {
public:
	FastEnemy(int x, int y);

	char getSymbol() const override;
	std::string getTypeName() const override;

	void takeTurn(const Point& heroPos, const Board& board) override;
};

// Tougher melee enemy: higher health and resists the Knight's Power
// Strike better than a regular Enemy, but still moves one step per turn.
//
// Orthogonal adjacency (one cell away, sharing a row or column) always
// resolves into a catch on Brute's very next step anyway, since movement
// is 4-directional -- there's no stable "adjacent but safe" state there,
// so dealing chip damage in that case would just be masked by the
// instant loss that follows immediately. Diagonal adjacency is different:
// it takes Brute two full steps to actually reach the hero from there, so
// it's a real window where Brute is "close" but can't yet catch you --
// Smash punishes lingering in that window instead of leaving it free.
class BruteEnemy : public Enemy {
public:
	BruteEnemy(int x, int y);

	char getSymbol() const override;
	std::string getTypeName() const override;

	int tryRangedAttack(const Point& heroPos, const Board& board) const override;
};

// Stationary archer: does not chase the hero. Instead, every turn it
// checks whether the hero is in the same row/column within range and
// with a clear line of sight (no walls in between); if so, it deals
// damage to the hero from a distance instead of moving.
class RangedEnemy : public Enemy {
	int range;
public:
	RangedEnemy(int x, int y);

	char getSymbol() const override;
	std::string getTypeName() const override;

	void takeTurn(const Point& heroPos, const Board& board) override;
	int tryRangedAttack(const Point& heroPos, const Board& board) const override;
};