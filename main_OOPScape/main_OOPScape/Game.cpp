#include "Game.h"
#include "Color.h"
#include <cctype>
#include <stdexcept>

Game::Game() : difficultyLevel(1) {}

void Game::loadLevel(const std::string& filename, int difficulty) {
    difficultyLevel = difficulty;
    board.loadFromFile(filename, difficulty);

    Point s = board.getStartPos();
    player = std::make_unique<Player>(s.getX(), s.getY(), 100, 1, Power("Teleport", 0), 'H');

    enemies.clear();
    const std::vector<Point>& positions = board.getEnemyPositions();
    for (int i = 0; i < (int)positions.size(); ++i) {
        const Point& ep = positions[i];

        if (i % 3 == 2) {
            enemies.push_back(std::make_unique<FastEnemy>(ep.getX(), ep.getY()));
        }
        else {
            enemies.push_back(std::make_unique<Enemy>(ep.getX(), ep.getY(), 50, 1, Power(), 'E'));
        }
    }
}


void Game::printStatus() const {
    Color::set(Color::BRIGHT_YELLOW);
    std::cout << "[ Health: " << player->getHealth() << " ]";
    if (player->canUseOOP()) {
        Color::set(Color::BRIGHT_CYAN);
        std::cout << "  [ OOP: READY ]";
    }
    else {
        Color::set(Color::DARK_GRAY);
        std::cout << "  [ OOP cooldown: " << player->getOopCooldown() << " ]";
    }
    Color::reset();
    std::cout << "\n";

    Color::set(Color::BRIGHT_CYAN);
    std::cout << "H";
    Color::reset();
    std::cout << "=Hero  ";
    Color::set(Color::BRIGHT_RED);
    std::cout << "E";
    Color::reset();
    std::cout << "=Enemy  ";
    Color::set(Color::BRIGHT_RED);
    std::cout << "F";
    Color::reset();
    std::cout << "=Fast enemy  ";
    Color::set(Color::BRIGHT_GREEN);
    std::cout << "F";
    Color::reset();
    std::cout << "=Exit\n";
    std::cout << "Commands: W/A/S/D to move, OOP to teleport, SAVE, QUIT\n";
}

static std::vector<Enemy> flatEnemies(const std::vector<std::unique_ptr<Enemy>>& v) {
    std::vector<Enemy> out;
    for (const auto& e : v) out.push_back(*e);
    return out;
}

void Game::run() {
    while (true) {

        auto flat = flatEnemies(enemies);
        board.draw(player.get(), flat);
        printStatus();


        std::string command;
        std::cout << "> ";
        std::cin >> command;
        for (char& c : command) c = (char)toupper(c);


        if (command == "W" || command == "A" || command == "S" || command == "D") {
            int dx = 0, dy = 0;
            if (command == "W") dy = -1;
            else if (command == "S") dy = 1;
            else if (command == "A") dx = -1;
            else if (command == "D") dx = 1;

            int nx = player->getX() + dx;
            int ny = player->getY() + dy;
            if (board.isWalkable(nx, ny)) {
                player->setX(nx);
                player->setY(ny);
            }
            else {
                Color::set(Color::BRIGHT_YELLOW);
                std::cout << "Can't move there (wall).\n";
                Color::reset();
                continue;
            }
        }
        else if (command == "OOP") {
            if (!player->canUseOOP()) {
                Color::set(Color::BRIGHT_YELLOW);
                std::cout << "OOP on cooldown (" << player->getOopCooldown() << " turns left).\n";
                Color::reset();
                continue;
            }
            if (player->useOOP(board, flat)) {
                Color::set(Color::BRIGHT_CYAN);
                std::cout << "*** TELEPORTED! ***\n";
                Color::reset();
            }
        }
        else if (command == "SAVE") {
            std::string saveName;
            std::cout << "Save name: ";
            std::cin >> saveName;
            saveGame(saveName);
            std::cout << "Game saved to '" << saveName << "'.\n";
            continue;
        }
        else if (command == "QUIT") {
            std::cout << "Quitting game.\n";
            return;
        }
        else {
            std::cout << "Unknown command. Use W/A/S/D, OOP, SAVE, QUIT.\n";
            continue;
        }


        if (checkWin()) {
            auto f2 = flatEnemies(enemies);
            board.draw(player.get(), f2);
            Color::set(Color::BRIGHT_GREEN);
            std::cout << "\n*** You reached the exit! You win! ***\n";
            Color::reset();
            return;
        }
        if (checkLose()) {
            auto f2 = flatEnemies(enemies);
            board.draw(player.get(), f2);
            Color::set(Color::BRIGHT_RED);
            std::cout << "\n*** An enemy caught you! Game over! ***\n";
            Color::reset();
            return;
        }


        player->tickCooldown();
        Point heroPos(player->getX(), player->getY());
        for (auto& e : enemies) {
            e->takeTurn(heroPos, board);
        }


        if (checkLose()) {
            auto f2 = flatEnemies(enemies);
            board.draw(player.get(), f2);
            Color::set(Color::BRIGHT_RED);
            std::cout << "\n*** An enemy caught you! Game over! ***\n";
            Color::reset();
            return;
        }
    }
}

bool Game::checkWin() const {
    Point exit = board.getExitPos();
    return player->getX() == exit.getX() && player->getY() == exit.getY();
}

bool Game::checkLose() const {
    for (const auto& e : enemies) {
        if (e->getX() == player->getX() && e->getY() == player->getY())
            return true;
    }
    return false;
}

void Game::saveGame(const std::string& saveName) const {
    std::ofstream save(saveName);
    if (!save.is_open())
        throw std::runtime_error("Could not open save file: " + saveName);

    save << difficultyLevel << "\n";
    save << player->getX() << " " << player->getY() << "\n";
    save << player->getOopCooldown() << "\n";
    save << enemies.size() << "\n";
    for (const auto& e : enemies) {

        char type = (dynamic_cast<FastEnemy*>(e.get())) ? 'F' : 'N';
        save << type << " " << e->getX() << " " << e->getY() << "\n";
    }
}

void Game::loadGame(const std::string& saveName) {
    std::ifstream save(saveName);
    if (!save.is_open())
        throw std::runtime_error("Could not open save file: " + saveName);

    save >> difficultyLevel;
    board.loadFromFile("Boards.txt", difficultyLevel);

    int heroX, heroY, cooldown;
    save >> heroX >> heroY >> cooldown;
    player = std::make_unique<Player>(heroX, heroY, 100, 1, Power("Teleport", 0), 'H');

    player->setOopCooldown(cooldown);


    int enemyCount;
    save >> enemyCount;
    enemies.clear();
    for (int i = 0; i < enemyCount; i++) {
        char type;
        int ex, ey;
        save >> type >> ex >> ey;
        if (type == 'F') {
            enemies.push_back(std::make_unique<FastEnemy>(ex, ey));
        }
        else {
            enemies.push_back(std::make_unique<Enemy>(ex, ey, 50, 1, Power(), 'E'));
        }
    }
}