//
// Created by pknadimp on 11/9/24.
//

#include "Game.h"
#include "move_generation.h"
#include "AI.h"
#include "Board.h"
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

bool Game::is_move_valid(const Piece &piece, int from_row, int from_col, int to_row, int to_col) const {
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

void Game::make_move(const Piece &piece, int from_row, int from_col, int to_row, int to_col) {
    auto moves = generate_moves(piece, m_board, Position{from_row, from_col});

    Move selected_move;
    for (auto move: moves) {
        print_move(move);
        if (move.to.row == to_row && move.to.col == to_col) {
            selected_move = move;
        }
    }
    m_board.make_move(selected_move);
    // m_board.set_piece(from_row, from_col, Piece{PieceType::NONE, PieceColor::NONE});
    // m_board.set_piece(to_row, to_col, piece);
    switch_turns();
}

void Game::make_computer_move(const PieceColor &color) {
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    auto move = generate_AI_move(m_board, color);
    m_board.make_move(move);
    switch_turns();
    //make_move(move.moved_piece, move.from.row, move.from.col, move.to.row, move.to.col);
}

