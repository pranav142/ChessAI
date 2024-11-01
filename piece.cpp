#include "piece.h"

// This seems like a bad way to manage pieces
PieceRule rules[] = {
    [static_cast<int>(PieceType::NONE)] = {' ', ""},
    [static_cast<int>(PieceType::PAWN)] = {'p', "pawn"},
    [static_cast<int>(PieceType::ROOK)] = {'r', "rook"},
    [static_cast<int>(PieceType::KNIGHT)] = {'n', "knight"},
    [static_cast<int>(PieceType::BISHOP)] = {'b', "bishop"},
    [static_cast<int>(PieceType::QUEEN)] = {'q', "queen"},
    [static_cast<int>(PieceType::KING)] = {'k', "king"},
};

PieceRule *get_piece_rule(PieceType piece_type) { return &rules[static_cast<int>(piece_type)]; }

Piece create_piece(const PieceType type, const PieceColor color) {
    Piece piece;
    piece.type = type;
    piece.color = color;
    return piece;
}

std::string piece_color_to_string(const PieceColor piece_color) {
    if (piece_color == PieceColor::WHITE) {
        return "white";
    } else if (piece_color == PieceColor::BLACK) {
        return "black";
    } else {
        return "none";
    }
}

