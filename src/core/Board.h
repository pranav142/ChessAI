#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <string>
#include "moves.h"
#include <SFML/Graphics.hpp>

#define BOARD_SIZE 8

#include <stack>

#include "castling_rights.h"
#include "FEN.h"

struct BoardState {
    CastlingRights castling_rights;
    Position en_passant_target;
};

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

    void set_to_previous_state();

    void unmake_move(const Move &move);

    void load_from_FEN(const FEN &fen);

    [[nodiscard]] Position get_en_passant_target() const;

    [[nodiscard]] bool has_castling_rights_queen_side(const PieceColor& color) const;

    [[nodiscard]] bool has_castling_rights_king_side(const PieceColor& color) const;

    bool is_move_safe_for_king(const Move &move, const PieceColor& piece_color) const;

    Position get_king_position(PieceColor piece_color) const;
private:
    static bool is_in_bounds(int row, int col);

    void update_castling_rights(const Move &move);

    void update_en_passant_target(const Move &move);

    void clear_en_passant_target();

    void set_en_passant_target(const Position &position);

    void save_state();

private:
    Piece m_board[BOARD_SIZE][BOARD_SIZE];

   std::stack<BoardState> m_state_stack;

    Piece m_empty_piece = Piece{PieceType::NONE, PieceColor::NONE};
    CastlingRights m_castling_rights;
    Position m_en_passant_target = Position{-1, -1};
};


bool is_valid_square_index(sf::Vector2i &square_index);

bool is_empty_square(sf::Vector2i &square_index, const Board &board);

#endif //BOARD_H
