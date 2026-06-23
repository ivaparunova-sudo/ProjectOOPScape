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
	Player();
	Player(int x, int y, int health, int speed, Power power, char symbol, int oopMaxCooldown = 5);
	virtual ~Player() = default;

	char getSymbol() const override;

	bool canUseOOP() const;
	int getOopCooldown() const;
	int getMaxHealth() const;
	bool isInvulnerable() const;

	void setOopCooldown(int cooldown);
	void tickCooldown();

	// Applies incoming damage, respecting invulnerability. Clamps health at 0.
	void takeDamage(int amount);
	void heal(int amount);

	// Decrements invulnerability counter; called once per full turn.
	void tickInvulnerability();
	void grantInvulnerability(int turns);

	// Name of the hero class, shown in the status bar (e.g. "Wizard").
	virtual std::string getClassName() const = 0;
	// Short label for the active ability, shown in the status bar (e.g. "Teleport").
	virtual std::string getAbilityName() const = 0;

	// Executes the hero's special ability. Returns true if it was used successfully.
	virtual bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) = 0;
};