#pragma once
#include "Point.h"
#include "Entity.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

class Enemy;

class Board {
    char grid[64][64];
    int size;
    Point startPos;
    Point exitPos;
    std::vector<Point> enemyPositions;

public:
    Board();

    int getSize() const;
    Point getStartPos() const;
    Point getExitPos() const;
    std::vector<Point> getEnemyPositions() const;

    void loadFromFile(const std::string& filename, int difficultyLevel);
    bool isWalkable(int nx, int ny) const;
    void draw(const Entity* player, const std::vector<Enemy>& enemies) const;
};
