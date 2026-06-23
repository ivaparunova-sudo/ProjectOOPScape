#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include "Board.h"
#include "Player.h"
#include "Enemy.h"

class Game {
    Board board;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    int difficultyLevel;

private:
    void printStatus() const;

public:
    Game();

    void loadLevel(const std::string& filename, int difficulty);
    void run();

    bool checkWin()  const;
    bool checkLose() const;

    void saveGame(const std::string& saveName) const;
    void loadGame(const std::string& saveName);

};