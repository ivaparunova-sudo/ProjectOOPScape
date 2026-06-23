#pragma once
#include "Player.h"
#include "Enemy.h"

class Wizard : public Player {
public:
	Wizard(int x, int y, int health);

	std::string getClassName() const override;
	std::string getAbilityName() const override;

	bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) override;
};

class Knight : public Player {
public:
	Knight(int x, int y, int health);

	std::string getClassName() const override;
	std::string getAbilityName() const override;

	bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) override;
};

class Healer : public Player {
	int invulnTurns;
public:
	Healer(int x, int y, int health);

	std::string getClassName() const override;
	std::string getAbilityName() const override;

	bool useOOP(const Board& board, const std::vector<Enemy*>& enemies) override;
};