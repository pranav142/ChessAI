#ifndef PIECE_H
#define PIECE_H

#include <string>

enum class PieceType {
    NONE,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
};

enum class PieceColor {
    NONE,
    WHITE,
    BLACK,
};

struct Piece {
    PieceType type = PieceType::NONE;
    PieceColor color = PieceColor::NONE;
    bool has_moved = false;
};

Piece create_piece(PieceType type, PieceColor color);

std::string piece_color_to_string(PieceColor piece_color);

std::string piece_type_to_char(PieceType type);

std::string piece_color_to_char(PieceColor color);

std::string piece_type_to_string(PieceType type);

void print_piece(const Piece &piece);

PieceColor get_enemy_color(const PieceColor &color);

#endif //PIECE_H
