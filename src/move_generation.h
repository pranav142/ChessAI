//
// Created by pknadimp on 11/19/24.
//

#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include <vector>
#include "moves.h"
#include "Board.h"

std::vector<Move> generate_moves(const Piece &piece, const Board &board, Position position);

bool is_piece_attacking_square(const Piece& piece, Position piece_position, Position target ,  const Board& board);

bool is_square_attacked_by_color(int row, int col, const PieceColor &color, const Board& board);

#endif //MOVE_GENERATION_H