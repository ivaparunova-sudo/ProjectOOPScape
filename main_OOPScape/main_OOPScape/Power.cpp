#include "Power.h"

Power::Power() : powerName("NONE"), damage(0) {}

Power::Power(std::string name, int dmg) : powerName(name), damage(dmg) {}

std::string Power::getPowerName() const
{
	return powerName;
}

int Power::getDamage() const
{
	return damage;
}

void Power::setPowerName(std::string pn)
{
	powerName = pn;
}

void Power::setDamage(int d)
{
	damage = d;
}