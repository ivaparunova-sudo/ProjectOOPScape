#include "Entity.h"

Entity::Entity() : x(0), y(0), health(0), speed(0), symbol('?') {}

Entity::Entity(int x, int y, int health, int speed, Power power, char symbol)
	: x(x), y(y), health(health), speed(speed), power(power), symbol(symbol) {
}

Entity::Entity(const Entity& other)
	: x(other.x), y(other.y), health(other.health), speed(other.speed), power(other.power), symbol(other.symbol) {
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

int Entity::getSpeed() const
{
	return speed;
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

void Entity::setSpeed(int s)
{
	speed = s;
}

void Entity::setPower(Power p)
{
	power = p;
}