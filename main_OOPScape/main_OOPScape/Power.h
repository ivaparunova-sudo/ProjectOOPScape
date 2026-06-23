#pragma once
#include <string>
class Power
{
private:
	std::string powerName;
	int damage;
public:

	Power();
	Power(std::string name, int dmg);

	std::string getPowerName() const;
	int getDamage() const;

	void setPowerName(std::string pn);
	void setDamage(int d);
};
