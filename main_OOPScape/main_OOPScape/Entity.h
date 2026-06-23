#pragma once
#include "Power.h"
class Entity
{
protected:
	int x, y, health, speed;
	Power power;
	char symbol;
public:

	Entity();
	Entity(int x, int y, int health, int speed, Power power, char symbol);
	Entity(const Entity& other);
	virtual ~Entity() = default;

	int getX() const;
	int getY() const;
	int getHealth() const;
	int getSpeed() const;
	Power getPower() const;

	void setX(int x);
	void setY(int y);
	void setHealth(int h);
	void setSpeed(int s);
	void setPower(Power p);

	virtual char getSymbol() const = 0;
};