#pragma once
#include "Player.h"
#include "Enemy.h"

// Wizard: teleports to the board cell that is farthest from every enemy
// (i.e. the safest reachable spot), on a cooldown. The original game's
// "Teleport" ability.
class Wizard : public Player {
public:
	Wizard();
	Wizard(int x, int y, int health, int speed);

	std::string getClassName() const override;
	std::string getAbilityName() const override;

	bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) override;
};

// Knight: Power Strike. Deals damage to every enemy within a 1-cell
// radius (up/down/left/right and diagonals) of the hero. Enemies can
// survive the hit if they have enough health left.
class Knight : public Player {
	int strikeDamage;
public:
	Knight();
	Knight(int x, int y, int health, int speed);

	std::string getClassName() const override;
	std::string getAbilityName() const override;

	bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) override;
};

// Healer: restores part of the hero's health and grants a few turns of
// invulnerability to all damage (contact and ranged).
class Healer : public Player {
	int healAmount;
	int invulnTurns;
public:
	Healer();
	Healer(int x, int y, int health, int speed);

	std::string getClassName() const override;
	std::string getAbilityName() const override;

	bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) override;
};