#include "Board.h"
#include "Enemy.h"
#include "Color.h"

Board::Board() : size(0) {}

int Board::getSize() const {
    return size;
}

Point  Board::getStartPos() const {
    return startPos;
}
Point  Board::getExitPos() const {
    return exitPos;
}
std::vector<Point>  Board::getEnemyPositions() const {
    return enemyPositions;
}

void Board::loadFromFile(const std::string& filename, int difficultyLevel) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open! '" + filename + "'\n");
    }

    std::string line;
    bool found = false;

    int mapsRead = 0;
    int lineNo = 0;
    while (std::getline(file, line)) {
        lineNo++;

        size_t first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) continue;
        std::string trimmed = line.substr(first);


        bool isNumber = !trimmed.empty();
        for (char c : trimmed) {
            if (!std::isdigit(static_cast<unsigned char>(c))) { isNumber = false; break; }
        }
        if (!isNumber) continue;

        int n = std::stoi(trimmed);
        if (n < 2 || n > 64)
            throw std::runtime_error("Invalid map size: " + std::to_string(n) +
                " (must be 2-64), at line " + std::to_string(lineNo));

        std::vector<std::string> rows;
        for (int i = 0; i < n; i++) {
            std::string row;
            if (!std::getline(file, row))
                throw std::runtime_error("Unexpected end of file while reading map of size " +
                    std::to_string(n) + " (expected " + std::to_string(n) + " rows, got " +
                    std::to_string(rows.size()));
            // Strip a trailing carriage return left over from CRLF line endings,
            // but otherwise the row's length must match n exactly -- it is NOT
            // padded or truncated, since that would silently hide malformed maps.
            if (!row.empty() && row.back() == '\r') row.pop_back();
            rows.push_back(row);
        }

        mapsRead++;
        if (mapsRead != difficultyLevel) continue;

        if ((int)rows.size() != n)
            throw std::runtime_error("Map size mismatch: expected " + std::to_string(n) + " rows, got " +
                std::to_string(rows.size()));

        for (int row = 0; row < n; row++) {
            if ((int)rows[row].size() != n)
                throw std::runtime_error("Row " + std::to_string(row) + " has length " +
                    std::to_string(rows[row].size()) + ", expected exactly " + std::to_string(n));
        }

        int countS = 0, countF = 0;
        enemyPositions.clear();

        for (int row = 0; row < n; row++) {
            const std::string& r = rows[row];

            for (int col = 0; col < n; col++) {
                char c = r[col];
                if (c != '*' && c != ' ' && c != 'S' && c != 'F' && c != 'E')
                    throw std::runtime_error(std::string("Invalid character '") + c + "' at row " +
                        std::to_string(row) + " col " + std::to_string(col));
                grid[row][col] = c;
                if (c == 'S') { countS++; startPos = Point(col, row); grid[row][col] = ' '; }
                if (c == 'F') { countF++; exitPos = Point(col, row); }
                if (c == 'E') { enemyPositions.push_back(Point(col, row)); grid[row][col] = ' '; }
            }
        }

        if (countS != 1)
            throw std::runtime_error("Map must have exactly one 'S' (found " + std::to_string(countS) + ")");
        if (countF != 1)
            throw std::runtime_error("Map must have exactly one 'F' (found " + std::to_string(countF) + ")");
        if (startPos == exitPos)
            throw std::runtime_error("Start position 'S' and exit 'F' cannot be the same cell");

        size = n;
        found = true;
        break;
    }

    if (!found)
        throw std::runtime_error("Map #" + std::to_string(difficultyLevel) + " not found in '" + filename + "'");
}

bool Board::isWalkable(int nx, int ny) const {
    if (nx < 0 || ny < 0 || nx >= size || ny >= size) return false;
    return grid[ny][nx] != '*';
}

void Board::draw(const Entity* player, const std::vector<Enemy*>& enemies) const {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {

            if (player->getX() == col && player->getY() == row) {
                Color::set(Color::BRIGHT_CYAN);
                std::cout << player->getSymbol();
                Color::reset();
                continue;
            }

            bool enemyHere = false;
            for (const Enemy* e : enemies) {
                if (!e->isAlive()) continue;
                if (e->getX() == col && e->getY() == row) {
                    Color::set(Color::BRIGHT_RED);
                    std::cout << e->getSymbol();
                    Color::reset();
                    enemyHere = true;
                    break;
                }
            }
            if (enemyHere) continue;

            char c = grid[row][col];
            if (c == '*') {
                Color::set(Color::DARK_GRAY);
                std::cout << c;
                Color::reset();
            }
            else if (c == 'F') {
                Color::set(Color::BRIGHT_GREEN);
                std::cout << c;
                Color::reset();
            }
            else {
                std::cout << c;
            }
        }
        std::cout << "\n";
    }
}