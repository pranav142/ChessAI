//
// Created by pknadimp on 12/2/24.
//

#ifndef FEN_H
#define FEN_H

#include <string>
#include "castling_rights.h"
#include "Piece.h"
#include "moves.h"

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

struct FEN {
    std::string board_string;
    CastlingRights castling_rights;
    Position en_passant_target;
    int half_move_clock;
    int full_move_number;
    PieceColor side_to_move;
};

bool create_fen_from_string(const std::string& fen_string, FEN& fen);
Piece piece_from_FEN_char(char c);

#endif //FEN_H
