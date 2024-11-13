#include <iostream>
#include "Piece.h"

#include "Board.h"

Board::Board() {
    initialize();
}

void Board::initialize() {
    constexpr PieceType back_row[BOARD_SIZE] = {
        PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
        PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
    };

    for (auto &row: m_board) {
        for (auto &square: row) {
            square = create_piece(PieceType::NONE, PieceColor::NONE);
        }
    }

    for (int col = 0; col < BOARD_SIZE; col++) {
        m_board[6][col] = create_piece(PieceType::PAWN, PieceColor::WHITE);
        m_board[1][col] = create_piece(PieceType::PAWN, PieceColor::BLACK);

        m_board[7][col] = create_piece(back_row[col], PieceColor::WHITE);
        m_board[0][col] = create_piece(back_row[col], PieceColor::BLACK);
    }
}

const Piece &Board::get_piece(int row, int col) const {
    if (!is_in_bounds(row, col)) {
        return m_empty_piece;
    }
    return m_board[row][col];
}

const Piece &Board::get_piece(const sf::Vector2i &square_index) const {
    return get_piece(square_index.x, square_index.y);
}

void Board::set_piece(int row, int col, const Piece &piece) {
    if (!is_in_bounds(row, col)) {
        return;
    }
    m_board[row][col] = piece;
}

bool Board::is_in_bounds(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

void Board::print_to_console() const {
    for (auto &row: m_board) {
        std::cout << '\n';
        for (const auto piece: row) {
            print_piece(piece);
            std::cout << ' ';
        }
    }
    std::cout << std::endl;
}

bool is_valid_square_index(sf::Vector2i &square_index) {
    return square_index.x >= 0 && square_index.x < BOARD_SIZE && square_index.y >= 0 && square_index.y < BOARD_SIZE;
}

bool is_empty_square(sf::Vector2i &square_index, const Board &board) {
    auto piece = board.get_piece(square_index);
    return piece.type == PieceType::NONE;
}

