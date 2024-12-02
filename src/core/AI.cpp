//
// Created by pknadimp on 11/19/24.
//

#include "AI.h"
#include "moves.h"
#include "move_generation.h"
#include "Piece.h"
#include <random>
#include <ctime>

Move generate_AI_move(Board &board, PieceColor pieceColor) {
    std::vector<Move> total_moves;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            auto piece = board.get_piece(i, j);
            if (piece.color == pieceColor) {
                auto moves = generate_moves(piece, board, Position{i, j});
                for (auto move: moves) {
                    total_moves.push_back(move);
                }
            }
        }
    }

    if (total_moves.size() <= 0) {
        return Move{};
    }

    std::mt19937 generator(std::time(0));
    std::uniform_int_distribution<int> distribution(0, total_moves.size() - 1);
    int randomIndex = distribution(generator);
    return total_moves[randomIndex];
}
