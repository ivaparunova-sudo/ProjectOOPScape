#pragma once
#include "Entity.h"
#include <vector>
#include <string>

class Board;
class Enemy;

// Abstract base for all playable heroes.
// Concrete heroes (Wizard, Knight, Healer) each implement their own
// special ability via useOOP(), which is the project's required
// "special ability" extension point (section 3.5 of the assignment).
class Player : public Entity
{
protected:
	int oopCooldown;
	int oopMaxCooldown;
	int maxHealth;
	int invulnerableTurns;   // > 0 means immune to damage for this many upcoming enemy turns

public:
	Player(int x, int y, int health, Power power, char symbol, int oopMaxCooldown = 5);
	virtual ~Player() = default;

	char getSymbol() const override;

	bool canUseOOP() const;
	int getOopCooldown() const;
	int getOopMaxCooldown() const;
	int getMaxHealth() const;
	bool isInvulnerable() const;

	void setOopCooldown(int cooldown);
	void tickCooldown();

	void takeDamage(int amount);
	void heal(int amount);

	void tickInvulnerability();
	void grantInvulnerability(int turns);

	virtual std::string getClassName() const = 0;
	virtual std::string getAbilityName() const = 0;

	virtual bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) = 0;
};