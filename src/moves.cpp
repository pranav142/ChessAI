//
// Created by pknadimp on 11/19/24.
//

#include "moves.h"

#include <iostream>

Move create_normal_move(const Position &from, const Position &to, const Piece &moved_piece) {
    Move move{};
    move.from = from;
    move.to = to;
    move.moved_piece = moved_piece;
    move.type = MoveType::NORMAL;
    return move;
}

Move create_castle_move(const Position &from, const Position &to, const Piece &moved_piece) {
    Move move{};
    move.from = from;
    move.to = to;
    move.moved_piece = moved_piece;
    move.type = MoveType::CASTLE;
    return move;
}

Move create_en_passant_move(const Position &from, const Position &to, const Piece &moved_piece, const Piece &captured_piece, const Position &capture_at) {
    Move move{};
    move.capture_at = capture_at;
    move.from = from;
    move.to = to;
    move.moved_piece = moved_piece;
    move.type = MoveType::EN_PASSANT;
    move.captured_piece = captured_piece;
    return move;
}

Move create_capture_move(const Position &from, const Position &to, const Piece &moved_piece, const Piece &captured_piece) {
    Move move{};
    move.from = from;
    move.to = to;
    move.moved_piece = moved_piece;
    move.captured_piece = captured_piece;
    move.capture_at = to;
    move.type = MoveType::CAPTURE;
    return move;
}

Move create_promotion_move(const Position &from, const Position &to, const Piece &moved_piece, const Piece &captured_piece, const Position &captured_at, const Piece &promotion_piece) {
    Move move{};
    move.from = from;
    move.to = to;
    move.moved_piece = moved_piece;
    move.captured_piece = captured_piece;
    move.capture_at = captured_at;
    move.promoted_piece = promotion_piece;
    move.type = MoveType::PROMOTION;
    return move;
}

void print_move(const Move &move) {
    std::cout << "Move Details:\n";
    std::cout << "  From: (" << move.from.row << ", " << move.from.col << ")\n";
    std::cout << "  To: (" << move.to.row << ", " << move.to.col << ")\n";

    if (move.type == MoveType::CAPTURE || move.type == MoveType::EN_PASSANT) {
        std::cout << "  Capture Position: (" << move.capture_at.row << ", " << move.capture_at.col << ")\n";
        std::cout << "  Captured Piece: "
                << piece_color_to_string(move.captured_piece.color) << " "
                << piece_type_to_string(move.captured_piece.type) << "\n";
    }

    std::cout << "  Moved Piece: "
            << piece_color_to_string(move.moved_piece.color) << " "
            << piece_type_to_string(move.moved_piece.type) << "\n";

    std::cout << "  Move Type: ";
    switch (move.type) {
        case MoveType::NORMAL: std::cout << "Normal\n";
            break;
        case MoveType::CAPTURE: std::cout << "Capture\n";
            break;
        case MoveType::CASTLE: std::cout << "Castling\n";
            break;
        case MoveType::PROMOTION: std::cout << "Promotion\n";
            break;
        case MoveType::EN_PASSANT: std::cout << "En Passant\n";
            break;
    }
}

