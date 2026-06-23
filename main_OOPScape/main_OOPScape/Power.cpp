#include "Power.h"

Power::Power(std::string name, int dmg) : powerName(name), damage(dmg) {}

std::string Power::getPowerName() const
{
	return powerName;
}

int Power::getDamage() const
{
	return damage;
}