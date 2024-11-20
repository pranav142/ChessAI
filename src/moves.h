//
// Created by pknadimp on 11/19/24.
//

#ifndef MOVES_H
#define MOVES_H

#include "Piece.h"

struct Position {
    int row;
    int col;
};


enum class MoveType {
    CASTLE,
    CAPTURE,
    PROMOTION,
    NORMAL,
    EN_PASSANT,
};

struct Move {
    Position from;
    Position to;
    Position capture_at;
    Piece captured_piece;
    Piece moved_piece;
    Piece promoted_piece;
    MoveType type;
};

void print_move(const Move &move);

Move create_normal_move(const Position &from, const Position &to, const Piece &moved_piece);

Move create_castle_move(const Position &from, const Position &to, const Piece &moved_piece);

Move create_en_passant_move(const Position &from, const Position &to, const Piece &moved_piece, const Piece &captured_piece, const Position &capture_at);

Move create_capture_move(const Position &from, const Position &to, const Piece &moved_piece, const Piece &captured_piece);

Move create_promotion_move(const Position &from, const Position &to, const Piece &moved_piece, const Piece &captured_piece, const Position &captured_at, const Piece &promotion_piece);

#endif //MOVES_H
