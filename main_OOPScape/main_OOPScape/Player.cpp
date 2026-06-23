#include "Player.h"

Player::Player()
    : Entity(0, 0, 100, 1, Power(), 'H'),
    oopCooldown(0), oopMaxCooldown(5), maxHealth(100), invulnerableTurns(0) {
}

Player::Player(int x, int y, int health, int speed, Power power, char symbol, int oopMaxCooldown)
    : Entity(x, y, health, speed, power, symbol),
    oopCooldown(0), oopMaxCooldown(oopMaxCooldown), maxHealth(health), invulnerableTurns(0) {
}

char Player::getSymbol() const { return symbol; }

bool Player::canUseOOP() const { return oopCooldown == 0; }
int  Player::getOopCooldown() const { return oopCooldown; }
int  Player::getMaxHealth() const { return maxHealth; }
bool Player::isInvulnerable() const { return invulnerableTurns > 0; }

void Player::setOopCooldown(int cooldown) { oopCooldown = cooldown; }

void Player::tickCooldown() {
    if (oopCooldown > 0) oopCooldown--;
}

void Player::takeDamage(int amount) {
    if (amount <= 0 || isInvulnerable()) return;
    health -= amount;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    if (amount <= 0) return;
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

void Player::tickInvulnerability() {
    if (invulnerableTurns > 0) invulnerableTurns--;
}

void Player::grantInvulnerability(int turns) {
    if (turns > invulnerableTurns) invulnerableTurns = turns;
}