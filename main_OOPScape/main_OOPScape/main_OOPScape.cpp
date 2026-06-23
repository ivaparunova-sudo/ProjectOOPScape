#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Game.h"
#include "Color.h"

static void printMenu() {
    Color::set(Color::BRIGHT_CYAN);
    std::cout << "\n=== OOPScape ===\n";
    Color::reset();
    std::cout << "(1) New game\n"
        << "(2) Load saved game\n"
        << "(3) Exit\n"
        << "Your choice: ";
}

static void newGame() {
    std::cout << "\nDifficulty:\n"
        << "  (1) Easy       10x10\n"
        << "  (2) Standard   20x20\n"
        << "  (3) Medium     30x30\n"
        << "  (4) Hard       40x40\n"
        << "  (5) Very Hard  50x50\n"
        << "  (6) Pro        60x60\n"
        << "Choose: ";

    int level;
    if (!(std::cin >> level)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid level.\n";
        return;
    }
    if (level < 1 || level > 6) {
        std::cout << "Invalid level.\n";
        return;
    }

    std::cout << "\nChoose your Hero Class:\n"
        << "  (1) Wizard\n"
        << "  (2) Knight\n"
        << "  (3) Healer\n"
        << "Choose: ";

    int classChoice;
    if (!(std::cin >> classChoice)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        classChoice = 1; // fall back to default (Wizard) below
    }

    HeroClass cls;
    if (classChoice == 2) {
        cls = HeroClass::Knight;
    }
    else if (classChoice == 3) {
        cls = HeroClass::Healer;
    }
    else {
        cls = HeroClass::Wizard; // Default or fallback selection
    }

    try {
        Game game;
        game.loadLevel("Boards.txt", level, cls);
        game.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}

static void loadGame() {
    std::cout << "Save file name: ";
    std::string name;
    if (!(std::cin >> name)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Could not read a save file name.\n";
        return;
    }

    try {
        Game game;
        game.loadGame(name);
        game.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Error loading game: " << ex.what() << "\n";
    }
}

int main() {
    int command = 0;
    while (command != 3) {
        printMenu();
        if (!(std::cin >> command)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Please enter a number.\n";
            continue;
        }
        switch (command) {
        case 1: newGame();  break;
        case 2: loadGame(); break;
        case 3:
            Color::set(Color::BRIGHT_CYAN);
            std::cout << "\nThanks for playing OOPScape! Goodbye!\n";
            Color::reset();
            break;
        default:
            std::cout << "Unknown command. Enter 1, 2 or 3.\n";
        }
    }
    return 0;
}