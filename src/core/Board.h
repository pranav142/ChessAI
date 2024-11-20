#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "moves.h"
#include <SFML/Graphics.hpp>

#define BOARD_SIZE 8


class Board {
public:
    Board();

    void initialize();

    void print_to_console() const;

    [[nodiscard]] const Piece &get_piece(int row, int col) const;

    [[nodiscard]] const Piece &get_piece(const sf::Vector2i &square_index) const;

    void set_piece(int row, int col, const Piece &piece);

    [[nodiscard]] bool is_empty(int row, int col) const;

    void make_move(const Move &move);

private:
    static bool is_in_bounds(int row, int col);

private:
    Piece m_board[BOARD_SIZE][BOARD_SIZE];

    Piece m_empty_piece = Piece{PieceType::NONE, PieceColor::NONE};
};


bool is_valid_square_index(sf::Vector2i &square_index);

bool is_empty_square(sf::Vector2i &square_index, const Board &board);

#endif //BOARD_H
