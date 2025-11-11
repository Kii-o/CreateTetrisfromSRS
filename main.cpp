/*
#include "Piece.hpp"

int main() {
    Game game;
    game.run();
    return 0;
}
*/

#include <iostream>
#include "Piece.hpp"
#include "Board.hpp"

int main() {

    Game game;
    Board board;

    // 現在のゲーム状態を取得
    GameState state = game.getGameState();

    std::cout << "Current Piece: " << pieceTypeToString(state.currentPiece) << std::endl;


    std::cout << "Next Queue: ";
    for (auto& p : state.nextPieces)
        std::cout << pieceTypeToString(p) << " ";
    std::cout << std::endl;

    std::cout << "Hold exists: " << (state.holdExists ? "true" : "false") << std::endl;
    std::cout << "Hold used this turn: " << (state.holdUsed ? "true" : "false") << std::endl;

    /*
    * ブロック設置
    board.placeBlock(3, 0, sf::Color::Red);
    board.placeBlock(4, 0, sf::Color::Red);
    board.placeBlock(5, 1, sf::Color::Red);
    */

    // 盤面を文字列として取得
    std::string boardState = board.toString();
    // 表示
    std::cout << boardState;

    return 0;
}
