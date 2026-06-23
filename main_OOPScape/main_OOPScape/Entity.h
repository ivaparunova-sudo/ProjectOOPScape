#pragma once
#include "Power.h"
class Entity
{
protected:
	int x, y, health;
	Power power;
	char symbol;
public:

	Entity(int x, int y, int health, Power power, char symbol);
	virtual ~Entity() = default;

	int getX() const;
	int getY() const;
	int getHealth() const;
	Power getPower() const;

	void setX(int x);
	void setY(int y);
	void setHealth(int h);
	void setPower(Power p);

	virtual char getSymbol() const = 0;
};