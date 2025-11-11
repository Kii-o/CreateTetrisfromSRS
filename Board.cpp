#include "Board.hpp"

// Blockクラスのコンストラクタ
Block::Block(bool f, sf::Color c) : filled(f), color(c) {}

// ブロックを描画する
void Block::draw(sf::RenderWindow& window, int x, int y, int size) {
    // 枠付きで描画するため、1px 小さくしている
    sf::RectangleShape rect(sf::Vector2f(size - 1, size - 1));
    rect.setPosition(x, y);
    rect.setFillColor(filled ? color : sf::Color(30, 30, 30)); // 空は濃いグレー
    window.draw(rect);
}

// Boardのコンストラクタ（空の20×10盤面を作る）
Board::Board() {
    grid.resize(HEIGHT, std::vector<Block>(WIDTH));
}


// 盤面全体を描画（左下が(0,0)）
void Board::draw(sf::RenderWindow& window) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int drawY = (HEIGHT - 1 - y);
            grid[y][x].draw(window, x * 40, drawY * 40);
        }
    }
}

// 指定座標にブロックがあるかどうかを判定
bool Board::isOccupied(int x, int y) {
    // 横がはみ出したらtrue（移動できない）
    if (x < 0 || x >= WIDTH || y >= HEIGHT) return true;
    // 下側（y < 0）は盤面外なのでfalse
    if (y < 0) return false;

    // 座標反転してチェック
    int gridY = HEIGHT - 1 - y;
    return grid[gridY][x].filled;
}

// 指定座標にブロックを配置する
void Board::placeBlock(int x, int y, sf::Color color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        int gridY = HEIGHT - 1 - y;  // ← 反転
        grid[gridY][x].filled = true;
        grid[gridY][x].color = color;
    }

    print();
}

// 揃ったラインを削除し、削除した行数を返す
int Board::clearLines() {
    int linesCleared = 0;

    // 下から上にチェックする（左下原点: y=0 が一番下(になるはずだったが多分左上が(0, 0)になってる)）
    for (int y = 0; y < HEIGHT; ++y) {
        bool full = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (!grid[y][x].filled) full = false;
        }

        if (full) {
            ++linesCleared;

            // この行を削除し、上の行を下に落とす
            for (int row = y; row < HEIGHT - 1; ++row) {
                for (int x = 0; x < WIDTH; ++x) {
                    grid[row][x] = grid[row + 1][x];
                }
            }

            // 一番上の行を空にする
            for (int x = 0; x < WIDTH; ++x)
                grid[HEIGHT - 1][x] = Block(false, sf::Color::Black);

            // 同じ行 y を再チェック（連続消去対応）
            --y;
        }
    }

    return linesCleared;
}

// ターミナルに盤面を出力する
void Board::print() {
#ifdef _WIN32
    system("cls");   // Windows
#else
    system("clear"); // macOS / Linux
#endif

    //std::cout << "===== BOARD =====" << std::endl;

    for (int y = HEIGHT - 1; y >= 0; --y) {
        bool fullLine = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (!grid[y][x].filled) {
                fullLine = false;
                break;
            }
        }

        // すべて埋まっている行はスキップ
        if (fullLine) continue;

        // 通常表示
        std::cout << "|";
        for (int x = 0; x < WIDTH; ++x) {
            std::cout << (grid[y][x].filled ? "X" : "_");
        }
        std::cout << "|" << std::endl;
    }

    //std::cout << "+" << std::string(WIDTH, '-') << "+" << std::endl;
}

// 盤面を文字列として返す
std::string Board::toString() const {
    std::string result;

    for (int y = HEIGHT - 1; y >= 0; --y) {
        bool fullLine = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (!grid[y][x].filled) {
                fullLine = false;
                break;
            }
        }

        // すべて埋まっている行はスキップ
        if (fullLine) continue;

        result += "|";
        for (int x = 0; x < WIDTH; ++x) {
            result += (grid[y][x].filled ? "X" : "_");
        }
        result += "|\n";
    }

    return result;
}
