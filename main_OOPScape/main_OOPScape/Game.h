#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include "Board.h"
#include "Player.h"
#include "Heroes.h"
#include "Enemy.h"

enum class HeroClass { Wizard, Knight, Healer };

class Game {
    Board board;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    int difficultyLevel;
    HeroClass heroClass;

private:
    void printStatus() const;
    std::vector<Enemy*> liveEnemyPointers() const;
    void resolveEnemyTurns();
    static std::unique_ptr<Player> makeHero(HeroClass cls, int x, int y);
    static std::unique_ptr<Enemy> makeEnemyByType(char type, int x, int y);
    static char enemyTypeCode(const Enemy& e);
    static char heroClassCode(HeroClass cls);
    static HeroClass heroClassFromCode(char code);

public:
    Game();

    void loadLevel(const std::string& filename, int difficulty, HeroClass cls);
    void run();

    bool checkWin()  const;
    bool checkLose() const;

    void saveGame(const std::string& saveName) const;
    void loadGame(const std::string& saveName);

};