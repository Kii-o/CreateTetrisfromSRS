#include "Piece.hpp" 
#include "Board.hpp"
#include <algorithm> 
#include <iostream> 
#include <array>
#include <vector>

// ==== 各ピースの形状定義 ====
// 各ピースは「4つの相対座標」で構成される
const std::array<std::array<sf::Vector2i, 4>, 7> PIECE_SHAPES = { {
    { sf::Vector2i(-1,0), sf::Vector2i(0,1), sf::Vector2i(0,0), sf::Vector2i(1,0) }, // Tミノ 
    { sf::Vector2i(-1,1), sf::Vector2i(0,1), sf::Vector2i(0,0), sf::Vector2i(1,0) }, // Zミノ
    { sf::Vector2i(-1,0), sf::Vector2i(0,1), sf::Vector2i(0,0), sf::Vector2i(1,1) }, // Sミノ
    { sf::Vector2i(-1,0), sf::Vector2i(0,0), sf::Vector2i(1,0), sf::Vector2i(2,0) }, // Iミノ
    { sf::Vector2i(0,1), sf::Vector2i(0,0), sf::Vector2i(1,1), sf::Vector2i(1,0) },  // Oミノ
    { sf::Vector2i(-1,1), sf::Vector2i(-1,0), sf::Vector2i(0,0), sf::Vector2i(1,0) },// Jミノ
    { sf::Vector2i(-1,0), sf::Vector2i(0,0), sf::Vector2i(1,1), sf::Vector2i(1,0) }  // Lミノ
} };

// ==== 各ピースの色定義 ====
const std::array<sf::Color, 7> PIECE_COLORS = {
    sf::Color(255,0,255),   // T = 紫
    sf::Color::Green,       // S = 緑
    sf::Color::Red,         // Z = 赤
    sf::Color::Cyan,        // I = 水色
    sf::Color::Yellow,      // O = 黄色
    sf::Color(255,165,0),   // J = 青
    sf::Color::Blue        // L = オレンジ
};

std::string toString(PieceType t) {
    switch (t) {
    case PieceType::I: return "I";
    case PieceType::O: return "O";
    case PieceType::T: return "T";
    case PieceType::S: return "S";
    case PieceType::Z: return "Z";
    case PieceType::J: return "J";
    case PieceType::L: return "L";
    default: return "?";
    }
}

// ピースの全ブロックの絶対座標を返す
std::array<sf::Vector2i, 4> Piece::getAbsolutePositions() const {
    std::array<sf::Vector2i, 4> abs;
    for (int i = 0; i < 4; i++) {
        abs[i] = { x + blocks[i].x, y + blocks[i].y };
    }
    return abs;
}

// ==================== Piece クラス ==================== 
// コンストラクタ：種類に応じて色と形を設定
Piece::Piece(PieceType type) {
    this->type = type;
    color = PIECE_COLORS[(int)type];
    blocks = PIECE_SHAPES[(int)type];
}

// フィールド上に現在のピースを描画
void Piece::draw(sf::RenderWindow& window) {
    for (auto& b : blocks) {
        int px = (x + b.x) * 40;   // 盤面上の描画位置X
        int py = (y + b.y) * 40;   // 盤面上の描画位置Y
        sf::RectangleShape rect(sf::Vector2f(39, 39)); // マスサイズ(39x39)の四角形
        rect.setPosition(px, py);
        rect.setFillColor(color);
        window.draw(rect);
    }
}

// Next / Hold用の小さなプレビュー描画
void Piece::drawPreview(sf::RenderWindow& window, int px, int py, int size) {
    for (auto& b : blocks) {
        sf::RectangleShape rect(sf::Vector2f(size - 1, size - 1));
        rect.setPosition(px + b.x * size, py + b.y * size);
        rect.setFillColor(color);
        window.draw(rect);
    }
}

// 指定した移動量 (dx,dy) で動けるかどうか判定
bool Piece::canMove(Board& board, int dx, int dy) {
    for (auto& b : blocks) {
        int nx = x + b.x + dx, ny = y + b.y + dy;
        if (board.isOccupied(nx, ny)) return false; // 盤面外またはブロック衝突 
    }
    return true;
}

// 実際にピースを移動する
void Piece::move(int dx, int dy) {
    x += dx;
    y += dy;
}

// // ------------------- ウォールキックテーブル -------------------
/*
// T, J, L, S, Z 用
const std::array<std::array<sf::Vector2i, 5>, 4> JLSTZ_OFFSET_TABLE = { {
    {{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {0,0}, {1,0}, {1,-1}, {0,2}, {1,2} }},
    {{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2} }}
} };
// I 用
const std::array<std::array<sf::Vector2i, 5>, 4> I_OFFSET_TABLE = { {
    {{ {0,0}, {-1,0}, {2,0}, {-1,0}, {2,0} }},
    {{ {-1,0}, {0,0}, {0,0}, {0,1}, {0,-2} }},
    {{ {-1,1}, {1,1}, {-2,1}, {1,0}, {-2,0} }},
    {{ {0,1}, {0,1}, {0,1}, {0,-1}, {0,2} }},
} };

// O 用
const std::array<std::array<sf::Vector2i, 5>, 4> O_OFFSET_TABLE = { {
    {{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {0,-1},{0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {-1,-1}, {0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {-1,0},{0,0}, {0,0}, {0,0}, {0,0} }}
} };
*/

// // ------------------- ウォールキックテーブル -------------------
// T, J, L, S, Z 用
const std::array<std::array<sf::Vector2i, 5>, 4> JLSTZ_OFFSET_TABLE = { {
    {{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {0,0}, {1,0}, {1,1}, {0,-2}, {1,-2} }},
    {{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2} }}
} };
// I 用
const std::array<std::array<sf::Vector2i, 5>, 4> I_OFFSET_TABLE = { {
    {{ {0,0}, {-1,0}, {2,0}, {-1,0}, {2,0} }},
    {{ {-1,0}, {0,0}, {0,0}, {0,-1}, {0,2} }},
    {{ {-1,-1}, {1,-1}, {-2,-1}, {1,0}, {-2,0} }},
    {{ {0,-1}, {0,-1}, {0,-1}, {0,1}, {0,-2} }},
} };

// O 用
const std::array<std::array<sf::Vector2i, 5>, 4> O_OFFSET_TABLE = { {
    {{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {0,-1},{0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {-1,-1}, {0,0}, {0,0}, {0,0}, {0,0} }},
    {{ {-1,0},{0,0}, {0,0}, {0,0}, {0,0} }}
} };

// 各テトリミノと対応するオフセットテーブルをマッピング
inline const std::array<std::array<sf::Vector2i, 5>, 4>& getWallKickTable(PieceType type) {
    switch (type) {
    case PieceType::I:
        return I_OFFSET_TABLE;
    case PieceType::O:
        return O_OFFSET_TABLE;
    default: // T, J, L, S, Z 共通
        return JLSTZ_OFFSET_TABLE;
    }
}

// --- ピースの座標を回転させる関数（時計回りにrotation回数分） ---
sf::Vector2i rotatedCell(int x, int y, int rotation) {
    for (int i = 0; i < rotation; i++) {
        int tmp = x;
        x = y;
        y = -tmp;
    }
    return sf::Vector2i(x, y);
}

// --- テトリミノの全ブロック座標を取得する関数 ---
std::array<sf::Vector2i, 4> Piece::getRotatedCells(int rotationState) const {
    std::array<sf::Vector2i, 4> cells;
    const auto& shape = PIECE_SHAPES[static_cast<int>(type)];
    for (int i = 0; i < 4; i++) {
        cells[i] = rotatedCell(shape[i].x, shape[i].y, rotationState);
    }
    return cells;
}

// --- ピースが盤面または壁と衝突しているかチェック ---
bool Piece::collides(Board& board, int xOffset, int yOffset, int rotationState) const {
    auto cells = getRotatedCells(rotationState);
    for (auto& c : cells) {
        int px = x + c.x + xOffset;
        int py = y + c.y + yOffset;
        // 範囲外 or 他のブロックに衝突
        if (px < 0 || px >= Board::WIDTH || py < 0 || py >= Board::HEIGHT)
            return true;
        if (board.isOccupied(px, py))
            return true;
    }
    return false;
}

// --- 回転処理（JSのrotatedPieceに相当） ---
void Piece::rotate(Board& board, bool clockwise) {
    int dir = clockwise ? 1 : -1;
    int newRot = (static_cast<int>(rotation) + dir + 4) % 4;

    // 回転前と後のオフセット表を取得
    const auto& offsetTable = getWallKickTable(type);
    const auto& fromTable = offsetTable[static_cast<int>(rotation)];
    const auto& toTable = offsetTable[newRot];

    // 各候補のオフセットを順に試す
    bool rotated = false;
    for (int i = 0; i < 5; i++) {
        sf::Vector2i from = fromTable[i];
        sf::Vector2i to = toTable[i];

        int dx = from.x - to.x;
        int dy = from.y - to.y;

        // 衝突判定
        if (!collides(board, dx, dy, newRot)) {
            // 回転確定
            rotation = static_cast<Rotation>(newRot);
            x += dx;
            y += dy;

            // blocksを回転形状に更新
            blocks = getRotatedCells(static_cast<int>(rotation));

            rotated = true;
            break;
        }
    }

    if (!rotated) {
        // 回転できない場合、何もしない
        std::cout << "Rotation blocked.\n";
    }
    else {
        std::cout << "Rotation succeeded.\n";
    }
}

void Piece::place(Board& board) {
    for (auto& b : blocks)
        board.placeBlock(x + b.x, y + b.y, color);
}

// ==================== Bag クラス ==================== 
// コンストラクタ：乱数生成器を初期化し、バッグをシャッフル
Bag::Bag() : rng(std::random_device{}()) {
    shuffleBag();
}

// 7種類のピースを袋に詰めてシャッフル
void Bag::shuffleBag() {
    pieces = { PieceType::T, PieceType::S, PieceType::Z, PieceType::I,
               PieceType::O, PieceType::J, PieceType::L };
    std::shuffle(pieces.begin(), pieces.end(), rng);
}

// 次のピースを1つ取り出す
PieceType Bag::getNext() {
    if (pieces.empty()) shuffleBag(); // 袋が空なら補充
    PieceType p = pieces.back();      // 最後の1つを取り出す
    pieces.pop_back();
    return p;
}

// ==================== Game クラス ==================== 
// コンストラクタ：ウィンドウ生成、ピース初期化、Nextキュー準備
Game::Game()
    : window(sf::VideoMode(Board::WIDTH * 40 + 200, Board::HEIGHT * 40), "Tetris"),
    currentPiece(bag.getNext())

{
    //std::cout << "コンストラクタ: Current piece is " << toString(currentPiece.type) << std::endl;
    // Nextキューに最初の5つを補充
    for (int i = 0; i < 5; ++i) nextQueue.push_back(bag.getNext());
}

// メインループ（イベント処理・入力処理・落下処理・描画を繰り返す）
void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        handleInput();
        handleFall();
        render();

        /*

        // --- デバッグ出力 ---
        auto abs = currentPiece.getAbsolutePositions();
        std::cout << "Current piece absolute positions: ";
        for (auto& p : abs) {
            std::cout << "(" << p.x << "," << p.y << ") ";
        }
        std::cout << std::endl;

        */
    }
}

// イベント処理（ウィンドウを閉じるなど）
void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event))
        if (event.type == sf::Event::Closed) window.close();
}

void Game::handleInput() {
    // 横移動はmoveIntervalで制限
    if (moveClock.getElapsedTime().asSeconds() < moveInterval) return;

    // --- 左右移動 ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        if (currentPiece.canMove(board, -1, 0)) currentPiece.move(-1, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        if (currentPiece.canMove(board, 1, 0)) currentPiece.move(1, 0);

    // --- 下移動 ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        if (currentPiece.canMove(board, 0, 1)) currentPiece.move(0, 1);

    // --- 回転 ---
    //SFMLの関係上逆にする必要がある
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        currentPiece.rotate(board, true); // 左回転
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        currentPiece.rotate(board, false);  // 右回転

    // --- 上移動（↑キーで1段上げる） ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        if (currentPiece.canMove(board, 0, -1)) currentPiece.move(0, -1);

    // --- ハードドロップ（スペースキー） ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        while (currentPiece.canMove(board, 0, 1)) {
            currentPiece.move(0, 1);  // 一番下まで落とす
        }
        currentPiece.place(board);     // 盤面に固定
        int lines = board.clearLines(); // ライン消去
        // 次のピース生成などの処理
        currentPiece = Piece(nextQueue.front());
        nextQueue.pop_front();
        nextQueue.push_back(bag.getNext());
        holdUsed = false;
    }

    // --- Hold機能 --- 
// Cキーが押されていて、まだこのターンでHoldを使っていない場合のみ処理
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !holdUsed) {
        if (!holdExists) {
            // === 初回ホールド ===
            // 現在のピースの種類をhold用変数に保存
            holdPiece = currentPiece;
            holdPieceType = currentPiece.type;
            // holdにピースが存在することを記録
            holdExists = true;
            // 現在のピースを「Nextキューの先頭のピース」に置き換える
            currentPiece = Piece(nextQueue.front());
            std::cout << "Current piece is " << toString(currentPiece.type) << std::endl;
            // Nextキューの先頭を削除（消費したので）
            nextQueue.pop_front();
            // 新しくbagから1つ取り出してNextキューの末尾に追加
            nextQueue.push_back(bag.getNext());
            std::cout << "Hold piece is " << toString(holdPiece->type) << std::endl;
        }
        else {
            // === 2回目以降のHold ===
            // 現在のピースの種類を一時的に保存
            std::cout << "before swap " << toString(holdPiece->type) << std::endl;
            Piece temppiece = currentPiece;
            PieceType temptype = currentPiece.type;
            // 一時保存しておいた種類をholdに戻す（入れ替え完了）
            // hold中のピースを現在のピースとして生成し直す
            currentPiece = Piece(holdPieceType);
            holdPiece = temppiece;
            holdPieceType = temptype;
            std::cout << "Hold piece is " << toString(holdPiece->type) << std::endl;
            std::cout << "Current piece is " << toString(currentPiece.type) << std::endl;
        }

        // 新しく取り出した/入れ替えたピースを初期位置(左上からx=3,y=0)に配置
        currentPiece.x = 3;
        currentPiece.y = 0;
        // このターンではもうHoldを使えないようにフラグを立てる
        holdUsed = true;
        //std::cout << "Current piece is " << toString(currentPiece.type) << std::endl;
    }

    // 移動入力のタイマーをリセット（Cを押した直後に再度連続入力されないようにする）
    moveClock.restart();

}


// 自動落下処理
void Game::handleFall() {
    if (fallClock.getElapsedTime().asSeconds() >= fallInterval) {
        if (currentPiece.canMove(board, 0, 1))
            currentPiece.move(0, 1);
        else {
            // 動けない＝着地 → 盤面に固定
            currentPiece.place(board);
            int lines = board.clearLines(); // ライン消去
            // 次のピースをセット
            currentPiece = Piece(nextQueue.front());
            nextQueue.pop_front();
            nextQueue.push_back(bag.getNext());
            holdUsed = false; // ホールド使用可能に戻す
        }
        fallClock.restart();
    }
}

// 描画処理
void Game::render() {
    window.clear();
    board.draw(window);         // 盤面
    currentPiece.draw(window);  // 現在のピース
    //std::cout << "Current piece is " << toString(currentPiece.type) << std::endl;

    // --- Next5の表示 ---
    int px = Board::WIDTH * 40 + 20, py = 20;
    int i = 0;
    for (auto& pType : nextQueue) {
        Piece p(pType);
        p.drawPreview(window, px, py + i * 100);
        ++i;
    }

    // --- Holdの表示 ---
    if (holdPiece) {  // has_value() の糖衣構文
        Piece p(*holdPiece);  // *で中身を取り出す
        p.drawPreview(window, px, 600);
    }

    window.display();
}

//main関数に現在のミノ、ネクスト、Bag数を返却する
GameState Game::getGameState() const {
    GameState state;
    state.currentPiece = currentPiece.type;
    state.holdExists = holdExists;
    state.holdUsed = holdUsed;

    // Nextキューの先頭から最大5個をコピー
    int i = 0;
    for (auto& p : nextQueue) {
        if (i >= 5) break;
        state.nextPieces[i++] = p;
    }

    return state;
}
