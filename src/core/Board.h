#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <string>
#include "moves.h"
#include <SFML/Graphics.hpp>

#define BOARD_SIZE 8

// Simply stores right to castle not the ability to castle
struct CastlingRights {
    bool white_king_side;
    bool white_queen_side;
    bool black_king_side;
    bool black_queen_side;
};

void reset_castling_rights(CastlingRights &rights);

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

    void load_from_FEN(const std::string &FEN);

    [[nodiscard]] Position get_en_passant_target() const;

    [[nodiscard]] bool has_castling_rights_queen_side(const PieceColor& color) const;

    [[nodiscard]] bool has_castling_rights_king_side(const PieceColor& color) const;

private:
    static bool is_in_bounds(int row, int col);

    void update_castling_rights(const Move &move);

    void update_en_passant_target(const Move &move);

    void clear_en_passant_target();

    void set_en_passant_target(const Position &position);

private:
    Piece m_board[BOARD_SIZE][BOARD_SIZE];

    Piece m_empty_piece = Piece{PieceType::NONE, PieceColor::NONE};
    CastlingRights m_castling_rights;
    Position m_en_passant_target = Position{-1, -1};
};


bool is_valid_square_index(sf::Vector2i &square_index);

bool is_empty_square(sf::Vector2i &square_index, const Board &board);

#endif //BOARD_H
