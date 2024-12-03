//
// Created by pknadimp on 12/2/24.
//

#include "FEN.h"


#include <sstream>
#include <string>
#include <vector>
#include <sstream>
#include <string>
#include <vector>

bool create_fen_from_string(const std::string &fen_string, FEN &fen) {
    std::istringstream fen_stream(fen_string);
    std::string segment;

    if (!(fen_stream >> segment)) {
        return false;
    }
    fen.board_string = segment;

    if (!(fen_stream >> segment)) {
        return false;
    }
    if (segment == "w") {
        fen.side_to_move = PieceColor::WHITE;
    } else if (segment == "b") {
        fen.side_to_move = PieceColor::BLACK;
    } else {
        return false;
    }

    if (!(fen_stream >> segment)) {
        return false;
    }
    fen.castling_rights = CastlingRights{
        segment.find('K') != std::string::npos,
        segment.find('Q') != std::string::npos,
        segment.find('k') != std::string::npos,
        segment.find('q') != std::string::npos,
    };

    if (!(fen_stream >> segment)) {
        return false;
    }
    if (segment == "-") {
        fen.en_passant_target = {-1, -1};
    } else if (segment.size() == 2 && segment[0] >= 'a' && segment[0] <= 'h' && segment[1] >= '1' && segment[1] <= '8') {
        fen.en_passant_target = {segment[1] - '1', segment[0] - 'a'};
    } else {
        return false;
    }

    if (!(fen_stream >> segment)) {
        return false;
    }
    try {
        fen.half_move_clock = std::stoi(segment);
    } catch (...) {
        return false;
    }

    if (!(fen_stream >> segment)) {
        return false;
    }
    try {
        fen.full_move_number = std::stoi(segment);
    } catch (...) {
        return false;
    }

    return true;
}


Piece piece_from_FEN_char(char c) {
    PieceColor color = isupper(c) ? PieceColor::WHITE : PieceColor::BLACK;
    PieceType type = PieceType::NONE;
    switch (toupper(c)) {
        case 'P': {
            type = PieceType::PAWN;
            break;
        }
        case 'N': {
            type = PieceType::KNIGHT;
            break;
        }
        case 'K': {
            type = PieceType::KING;
            break;
        }
        case 'Q': {
            type = PieceType::QUEEN;
            break;
        }
        case 'B': {
            type = PieceType::BISHOP;
            break;
        }
        case 'R': {
            type = PieceType::ROOK;
            break;
        }
        default: ;
    }

    return Piece{type, color};
}
