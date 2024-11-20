#include "Piece.h"

#include <iostream>
#include "Board.h"

Piece create_piece(const PieceType type, const PieceColor color) {
    Piece piece;
    piece.type = type;
    piece.color = color;
    return piece;
}

std::string piece_color_to_string(const PieceColor piece_color) {
    if (piece_color == PieceColor::WHITE) {
        return "white";
    }
    if (piece_color == PieceColor::BLACK) {
        return "black";
    }
    return "none";
}

std::string piece_type_to_string(const PieceType type) {
    switch (type) {
        case PieceType::PAWN: return "pawn";
        case PieceType::KNIGHT: return "knight";
        case PieceType::BISHOP: return "bishop";
        case PieceType::ROOK: return "rook";
        case PieceType::QUEEN: return "queen";
        case PieceType::KING: return "king";
        case PieceType::NONE : return "none";
        default: return "none";
    }
}

std::string piece_type_to_char(const PieceType type) {
    switch (type) {
        case PieceType::NONE: return "U";
        case PieceType::PAWN: return "P";
        case PieceType::ROOK: return "R";
        case PieceType::KNIGHT: return "N";
        case PieceType::BISHOP: return "B";
        case PieceType::QUEEN: return "Q";
        case PieceType::KING: return "K";
        default: return "U";
    }
}

std::string piece_color_to_char(const PieceColor color) {
    switch (color) {
        case PieceColor::NONE: return "U";
        case PieceColor::WHITE: return "W";
        case PieceColor::BLACK: return "B";
        default: return "U";
    }
}

void print_piece(const Piece &piece) {
    std::cout << piece_color_to_char(piece.color) << piece_type_to_char(piece.type);
}

PieceColor get_enemy_color(const PieceColor &color) {
    return color == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;
}


