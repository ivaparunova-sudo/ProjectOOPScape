#include "Entity.h"

Entity::Entity(int x, int y, int health, Power power, char symbol)
	: x(x), y(y), health(health), power(power), symbol(symbol) {
}

int Entity::getX() const {
	return x;
}

int Entity::getY() const {
	return y;
}

int Entity::getHealth() const
{
	return health;
}

Power Entity::getPower() const
{
	return power;
}

void Entity::setX(int x) {
	this->x = x;
}

void Entity::setY(int y) {
	this->y = y;
}

void Entity::setHealth(int h)
{
	health = h;
}

void Entity::setPower(Power p)
{
	power = p;
}