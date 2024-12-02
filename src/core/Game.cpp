//
// Created by pknadimp on 11/9/24.
//

#include "Game.h"
#include "move_generation.h"
#include "AI.h"
#include "Board.h"
#include "move_generation.h"
#include <thread>

Game::Game() {}

void Game::initialize() {
    m_board.initialize();
    m_current_player = &m_white_player;
}

void Game::switch_turns() {
    m_current_player = m_current_player == &m_white_player ? &m_black_player : &m_white_player;
}

const Board &Game::get_board() const {
    return m_board;
}

const Player &Game::get_current_player() const {
    return *m_current_player;
}

bool Game::is_empty_square(int row, int col) const {
    return m_board.is_empty(row, col);
}

const Piece &Game::get_piece(int row, int col) const {
    return m_board.get_piece(row, col);
}

std::vector<Move> Game::get_moves(const Piece &piece, int row, int col) {
    return generate_moves(piece, m_board, Position{row, col});
}

std::vector<Move> Game::get_all_legal_moves() {
    std::vector<Move> total_moves;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            Piece piece = m_board.get_piece(row, col);
            if (piece.color == m_current_player->color) {
                auto moves = get_moves(piece, row, col);
                total_moves.insert(total_moves.end(), moves.begin(), moves.end());
            }
        }
    }

    return total_moves;
}

bool Game::is_move_valid(const Piece &piece, int from_row, int from_col, int to_row, int to_col) {
    if (piece.color != m_current_player->color) {
        return false;
    }

    auto moves = generate_moves(piece, m_board, Position{from_row, from_col});
    for (auto move: moves) {
        if (move.to.row == to_row && move.to.col == to_col) {
            return true;
        }
    }

    return false;
}

// TODO: Some way to ensure we always select a move
void Game::make_move(const Piece &piece, int from_row, int from_col, int to_row, int to_col, const Piece& promoted_piece) {
    auto moves = generate_moves(piece, m_board, Position{from_row, from_col});

    Move selected_move;
    for (auto move: moves) {
        if (move.to.row == to_row && move.to.col == to_col) {
            if (move.type == MoveType::PROMOTION && move.promoted_piece.type == promoted_piece.type && move.promoted_piece.color == promoted_piece.color) {
                selected_move = move;
                break;
            }
            selected_move = move;
        }
    }
    print_move(selected_move);
    m_board.make_move(selected_move);
    switch_turns();
}

void Game::set_FEN(std::string fen) {
    m_board.load_from_FEN(fen);
}

void Game::make_move(const Move &move) {
    m_board.make_move(move);
    switch_turns();
}

void Game::unmake_move(const Move& move) {
    m_board.unmake_move(move);
    switch_turns();
}

void Game::make_computer_move(const PieceColor &color) {
    auto move = generate_AI_move(m_board, color);
    m_board.make_move(move);
    switch_turns();
}

