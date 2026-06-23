#pragma once
#include "Player.h"
#include "Enemy.h"

// Wizard: teleports to the board cell that is farthest from every enemy
// (i.e. the safest reachable spot), on a cooldown. The original game's
// "Teleport" ability.
class Wizard : public Player {
public:
	Wizard(int x, int y, int health);

	std::string getClassName() const override;
	std::string getAbilityName() const override;

	bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) override;
};

// Knight: Power Strike. Deals damage to every enemy within a 1-cell
// radius (up/down/left/right and diagonals) of the hero. Enemies can
// survive the hit if they have enough health left. Strike damage comes
// from this hero's Power object (set in the constructor) instead of a
// separate hardcoded member, so it's one source of truth.
class Knight : public Player {
public:
	Knight(int x, int y, int health);

	std::string getClassName() const override;
	std::string getAbilityName() const override;

	bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) override;
};

// Healer: restores part of the hero's health and grants a few turns of
// invulnerability to all damage (contact and ranged). The heal amount
// comes from this hero's Power object instead of a separate hardcoded
// member, so it's one source of truth.
class Healer : public Player {
	int invulnTurns;
public:
	Healer(int x, int y, int health);

	std::string getClassName() const override;
	std::string getAbilityName() const override;

	bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) override;
};