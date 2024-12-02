#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "AI.h"
#include "moves.h"

// Decouple Game from AI?

enum class PlayerType {
    HUMAN,
    AI,
};

struct Player {
    PieceColor color;
    PlayerType type;
};

class Game {
public:
    Game();

    ~Game() = default;
    void initialize();

    [[nodiscard]] const Board &get_board() const;

    [[nodiscard]] const Player &get_current_player() const;

    [[nodiscard]] bool is_empty_square(int row, int col) const;

    [[nodiscard]] const Piece &get_piece(int row, int col) const;

    std::vector<Move> get_moves(const Piece& piece, int row, int col);

    std::vector<Move> get_all_legal_moves();

    bool is_move_valid(const Piece &piece, int from_row, int from_col, int to_row, int to_col);

    [[nodiscard]] bool is_move_valid(const Piece &piece, int from_row, int from_col, int to_row, int to_col) const;

    void make_move(const Piece &piece, int from_row, int from_col, int to_row, int to_col, const Piece &promoted_piece);

    void set_FEN(std::string fen);

    void make_move(const Move& move);

    void unmake_move(const Move &move);

    void make_computer_move(const PieceColor &color);

private:
    void switch_turns();

private:
    Board m_board;

    Player *m_current_player;
    Player m_black_player = Player{PieceColor::BLACK, PlayerType::HUMAN};
    Player m_white_player = Player{PieceColor::WHITE, PlayerType::HUMAN};
};

#endif //GAME_H
