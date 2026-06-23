#include "Game.h"
#include "Color.h"
#include <cctype>
#include <stdexcept>

namespace {
    // "A Brute Enemy" vs "An Enemy" -- the base Enemy's type name starts
    // with a vowel sound, the others don't, so this can't be hardcoded.
    std::string withIndefiniteArticle(const std::string& noun) {
        static const std::string vowels = "AEIOUaeiou";
        bool startsWithVowel = !noun.empty() && vowels.find(noun[0]) != std::string::npos;
        return (startsWithVowel ? "An " : "A ") + noun;
    }
}

Game::Game() : difficultyLevel(1), heroClass(HeroClass::Wizard) {}

std::unique_ptr<Player> Game::makeHero(HeroClass cls, int x, int y) {
    switch (cls) {
    case HeroClass::Knight: return std::make_unique<Knight>(x, y, 120);
    case HeroClass::Healer: return std::make_unique<Healer>(x, y, 90);
    case HeroClass::Wizard:
    default:                return std::make_unique<Wizard>(x, y, 100);
    }
}

char Game::heroClassCode(HeroClass cls) {
    switch (cls) {
    case HeroClass::Knight: return 'K';
    case HeroClass::Healer: return 'L';
    case HeroClass::Wizard:
    default:                return 'W';
    }
}

HeroClass Game::heroClassFromCode(char code) {
    switch (code) {
    case 'K': return HeroClass::Knight;
    case 'L': return HeroClass::Healer;
    case 'W':
    default:  return HeroClass::Wizard;
    }
}

std::unique_ptr<Enemy> Game::makeEnemyByType(char type, int x, int y) {
    switch (type) {
    case 'F': return std::make_unique<FastEnemy>(x, y);
    case 'B': return std::make_unique<BruteEnemy>(x, y);
    case 'R': return std::make_unique<RangedEnemy>(x, y);
    case 'N':
    default:  return std::make_unique<Enemy>(x, y, 50, Power("Bite", 0), 'E');
    }
}

char Game::enemyTypeCode(const Enemy& e) {
    if (dynamic_cast<const FastEnemy*>(&e))   return 'F';
    if (dynamic_cast<const BruteEnemy*>(&e))  return 'B';
    if (dynamic_cast<const RangedEnemy*>(&e)) return 'R';
    return 'N';
}

void Game::loadLevel(const std::string& filename, int difficulty, HeroClass cls) {
    difficultyLevel = difficulty;
    heroClass = cls;
    board.loadFromFile(filename, difficulty);

    Point s = board.getStartPos();
    player = makeHero(cls, s.getX(), s.getY());

    enemies.clear();
    const std::vector<Point>& positions = board.getEnemyPositions();
    for (int i = 0; i < (int)positions.size(); ++i) {
        const Point& ep = positions[i];

        // Distribute enemy variety across the spawn points found on the map:
        // every 4th enemy is a Brute, every 3rd (remaining) is Fast, every
        // 5th is Ranged, the rest are regular chasers. This guarantees at
        // least some variety without needing extra map syntax.
        char type = 'N';
        if (i % 5 == 4)      type = 'R';
        else if (i % 4 == 3) type = 'B';
        else if (i % 3 == 2) type = 'F';

        enemies.push_back(makeEnemyByType(type, ep.getX(), ep.getY()));
    }
}

std::vector<Enemy*> Game::liveEnemyPointers() const {
    std::vector<Enemy*> out;
    for (const auto& e : enemies) out.push_back(e.get());
    return out;
}

void Game::printStatus() const {
    Color::set(Color::BRIGHT_YELLOW);
    std::cout << "[ " << player->getClassName() << " ]  [ Health: " << player->getHealth()
        << "/" << player->getMaxHealth() << " ]";
    if (player->isInvulnerable()) {
        Color::set(Color::BRIGHT_CYAN);
        std::cout << "  [ INVULNERABLE ]";
    }
    if (player->canUseOOP()) {
        Color::set(Color::BRIGHT_CYAN);
        std::cout << "  [ " << player->getAbilityName() << ": READY ]";
    }
    else {
        Color::set(Color::DARK_GRAY);
        std::cout << "  [ " << player->getAbilityName() << " cooldown: " << player->getOopCooldown() << " ]";
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
    std::cout << "V";
    Color::reset();
    std::cout << "=Fast  ";
    Color::set(Color::BRIGHT_RED);
    std::cout << "B";
    Color::reset();
    std::cout << "=Brute  ";
    Color::set(Color::BRIGHT_RED);
    std::cout << "R";
    Color::reset();
    std::cout << "=Ranged  ";
    Color::set(Color::BRIGHT_GREEN);
    std::cout << "F";
    Color::reset();
    std::cout << "=Exit\n";
    std::cout << "Commands: L/R/U/D to move, OOP for ability, SAVE, QUIT\n";
}

void Game::run() {
    while (true) {

        board.draw(player.get(), liveEnemyPointers());
        printStatus();

        if (checkLose()) {
            Color::set(Color::BRIGHT_RED);
            std::cout << "\n*** " << describeLoss() << " Game over! ***\n";
            Color::reset();
            return;
        }

        std::string command;
        std::cout << "> ";
        if (!(std::cin >> command)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        for (char& c : command) c = (char)toupper(c);

        // Both WASD and the assignment's L/R/U/D scheme are accepted.
        if (command == "W" || command == "U" ||
            command == "A" || command == "L" ||
            command == "S" || command == "D" ||
            command == "R") {

            // Note: 'D' is ambiguous between "Down" (assignment scheme) and
            // "right" in WASD-only thinking, but since the assignment defines
            // D = down explicitly, that's the meaning used here; 'A' is left.
            int dx = 0, dy = 0;
            if (command == "W" || command == "U") dy = -1;
            else if (command == "S" || command == "D") dy = 1;
            else if (command == "A" || command == "L") dx = -1;
            else if (command == "R") dx = 1;

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
                std::cout << "Ability on cooldown (" << player->getOopCooldown() << " turns left).\n";
                Color::reset();
                continue;
            }
            auto liveEnemies = liveEnemyPointers();
            if (player->useOOP(board, liveEnemies)) {
                Color::set(Color::BRIGHT_CYAN);
                std::cout << "*** " << player->getAbilityName() << " used! ***\n";
                Color::reset();
            }
            else {
                Color::set(Color::BRIGHT_YELLOW);
                std::cout << "Ability had no effect.\n";
                Color::reset();
            }
        }
        else if (command == "SAVE") {
            std::string saveName;
            std::cout << "Save name: ";
            if (!(std::cin >> saveName)) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                Color::set(Color::BRIGHT_YELLOW);
                std::cout << "Could not read a save name.\n";
                Color::reset();
                continue;
            }
            try {
                saveGame(saveName);
                std::cout << "Game saved to '" << saveName << "'.\n";
            }
            catch (const std::exception& ex) {
                Color::set(Color::BRIGHT_YELLOW);
                std::cout << "Save failed: " << ex.what() << "\n";
                Color::reset();
            }
            continue;
        }
        else if (command == "QUIT") {
            std::cout << "Quitting game.\n";
            return;
        }
        else {
            std::cout << "Unknown command. Use L/R/U/D (or WASD), OOP, SAVE, QUIT.\n";
            continue;
        }


        if (checkLose()) {
            board.draw(player.get(), liveEnemyPointers());
            Color::set(Color::BRIGHT_RED);
            std::cout << "\n*** " << describeLoss() << " Game over! ***\n";
            Color::reset();
            return;
        }
        if (checkWin()) {
            board.draw(player.get(), liveEnemyPointers());
            Color::set(Color::BRIGHT_GREEN);
            std::cout << "\n*** You reached the exit! You win! ***\n";
            Color::reset();
            return;
        }


        player->tickCooldown();
        resolveEnemyTurns();
        player->tickInvulnerability();


        if (checkLose()) {
            board.draw(player.get(), liveEnemyPointers());
            Color::set(Color::BRIGHT_RED);
            std::cout << "\n*** " << describeLoss() << " Game over! ***\n";
            Color::reset();
            return;
        }
    }
}

void Game::resolveEnemyTurns() {
    Point heroPos(player->getX(), player->getY());
    for (auto& e : enemies) {
        if (!e->isAlive()) continue;

        if (e->isStunned()) {
            e->clearStun(); // stun lasts exactly the round it was inflicted
            continue;
        }

        // Attack and movement are independent: a stationary RangedEnemy's
        // takeTurn() is a no-op either way, and a BruteEnemy that smashes a
        // diagonally-adjacent hero still takes its normal step afterward --
        // only RangedEnemy is actually stationary, that's enforced by its
        // own takeTurn() override, not by skipping the call here.
        int dmg = e->tryRangedAttack(heroPos, board);
        if (dmg > 0) {
            // Only announce the hit if it actually lands -- takeDamage() is a
            // no-op while the hero is invulnerable, and saying "hits you for
            // N damage" when nothing happened would be misleading.
            if (!player->isInvulnerable()) {
                Color::set(Color::BRIGHT_RED);
                std::cout << withIndefiniteArticle(e->getTypeName()) << "'s " << e->getPower().getPowerName()
                    << " hits you for " << dmg << " damage!\n";
                Color::reset();
            }
            player->takeDamage(dmg);
        }
        e->takeTurn(heroPos, board);
    }
}

bool Game::checkWin() const {
    Point exit = board.getExitPos();
    return player->getX() == exit.getX() && player->getY() == exit.getY();
}

bool Game::checkLose() const {
    if (player->getHealth() <= 0) return true;
    for (const auto& e : enemies) {
        if (!e->isAlive()) continue;
        if (e->getX() == player->getX() && e->getY() == player->getY())
            return true;
    }
    return false;
}

std::string Game::describeLoss() const {
    if (player->getHealth() <= 0)
        return "Your health reached 0!";
    for (const auto& e : enemies) {
        if (!e->isAlive()) continue;
        if (e->getX() == player->getX() && e->getY() == player->getY())
            return withIndefiniteArticle(e->getTypeName()) + " caught you!";
    }
    return "Game over!"; // shouldn't normally be reached -- checkLose() found a reason, this is just a fallback
}

void Game::saveGame(const std::string& saveName) const {
    std::ofstream save(saveName);
    if (!save.is_open())
        throw std::runtime_error("Could not open save file: " + saveName);

    save << difficultyLevel << "\n";
    save << heroClassCode(heroClass) << "\n";
    save << player->getX() << " " << player->getY() << " " << player->getHealth() << "\n";
    save << player->getOopCooldown() << "\n";
    save << enemies.size() << "\n";
    for (const auto& e : enemies) {
        save << enemyTypeCode(*e) << " " << e->getX() << " " << e->getY()
            << " " << e->getHealth() << "\n";
    }
}

void Game::loadGame(const std::string& saveName) {
    std::ifstream save(saveName);
    if (!save.is_open())
        throw std::runtime_error("Could not open save file: " + saveName);

    if (!(save >> difficultyLevel))
        throw std::runtime_error("Save file corrupted: could not read difficulty level");
    if (difficultyLevel < 1 || difficultyLevel > 6)
        throw std::runtime_error("Save file corrupted: difficulty level " +
            std::to_string(difficultyLevel) + " is out of range (1-6)");

    char classCode;
    if (!(save >> classCode))
        throw std::runtime_error("Save file corrupted: could not read hero class code");
    if (classCode != 'K' && classCode != 'L' && classCode != 'W')
        throw std::runtime_error(std::string("Save file corrupted: unknown hero class code '") +
            classCode + "'");
    heroClass = heroClassFromCode(classCode);

    board.loadFromFile("Boards.txt", difficultyLevel);
    int n = board.getSize();

    int heroX, heroY, heroHealth, cooldown;
    if (!(save >> heroX >> heroY >> heroHealth >> cooldown))
        throw std::runtime_error("Save file corrupted: could not read hero state");
    if (heroX < 0 || heroY < 0 || heroX >= n || heroY >= n)
        throw std::runtime_error("Save file corrupted: hero position (" + std::to_string(heroX) +
            "," + std::to_string(heroY) + ") is outside the " + std::to_string(n) + "x" +
            std::to_string(n) + " board");
    if (!board.isWalkable(heroX, heroY))
        throw std::runtime_error("Save file corrupted: hero position (" + std::to_string(heroX) +
            "," + std::to_string(heroY) + ") is not a walkable cell");
    if (cooldown < 0)
        throw std::runtime_error("Save file corrupted: negative ability cooldown (" +
            std::to_string(cooldown) + ")");

    player = makeHero(heroClass, heroX, heroY);

    if (heroHealth < 0 || heroHealth > player->getMaxHealth())
        throw std::runtime_error("Save file corrupted: hero health " + std::to_string(heroHealth) +
            " is out of range (0-" + std::to_string(player->getMaxHealth()) + ")");
    player->setHealth(heroHealth);
    player->setOopCooldown(cooldown);

    int enemyCount;
    if (!(save >> enemyCount))
        throw std::runtime_error("Save file corrupted: could not read enemy count");
    if (enemyCount < 0 || enemyCount > n * n)
        throw std::runtime_error("Save file corrupted: invalid enemy count " +
            std::to_string(enemyCount));

    enemies.clear();
    for (int i = 0; i < enemyCount; i++) {
        char type;
        int ex, ey, eHealth;
        if (!(save >> type >> ex >> ey >> eHealth))
            throw std::runtime_error("Save file corrupted: could not read enemy #" +
                std::to_string(i));
        if (type != 'F' && type != 'B' && type != 'R' && type != 'N')
            throw std::runtime_error("Save file corrupted: enemy #" + std::to_string(i) +
                " has unknown type code '" + type + "'");
        if (ex < 0 || ey < 0 || ex >= n || ey >= n)
            throw std::runtime_error("Save file corrupted: enemy #" + std::to_string(i) +
                " position (" + std::to_string(ex) + "," + std::to_string(ey) +
                ") is outside the board");
        if (eHealth < 0)
            throw std::runtime_error("Save file corrupted: enemy #" + std::to_string(i) +
                " has negative health (" + std::to_string(eHealth) + ")");

        auto enemy = makeEnemyByType(type, ex, ey);
        enemy->setHealth(eHealth);
        enemies.push_back(std::move(enemy));
    }
}