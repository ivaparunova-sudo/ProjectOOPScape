#pragma once
#include "Entity.h"
#include "Board.h"

class Player : public Entity
{
	int oopCooldown;
	static const int OOP_MAX_COOLDOWN = 5;

public:
	Player();
	Player(int x, int y, int health, int speed, Power power, char symbol);

	char getSymbol() const override;

	bool canUseOOP() const;
	int getOopCooldown() const;

	void setOopCooldown(int cooldown);


	bool useOOP(const Board& board, const std::vector<class Enemy>& enemies);


	void tickCooldown();
};
