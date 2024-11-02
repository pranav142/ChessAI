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
};

typedef struct {
    char letter;
    std::string piece_name;
} PieceRule;

Piece create_piece(PieceType type, PieceColor color);

PieceRule *get_piece_rule(PieceType piece_type);

std::string piece_color_to_string(PieceColor piece_color);

void print_piece(const Piece& piece);

#endif //PIECE_H