#include <iostream>
#include "Piece.h"
#include "Board.h"

void reset_castling_rights(CastlingRights &rights) {
    rights.black_king_side = true;
    rights.white_king_side = true;
    rights.white_queen_side = true;
    rights.black_queen_side = true;
}

Board::Board() {
    initialize();
}

void Board::initialize() {
    load_from_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // load_from_FEN("r3k2r/1P6/8/8/8/8/8/R3K2R w KQkq - 0 1");
    // load_from_FEN("r3k2r/8/8/8/8/8/1p6/R3K2R w KQkq - 0 1");
    reset_castling_rights(m_castling_rights);
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

bool Board::is_empty(int row, int col) const {
    return get_piece(row, col).type == PieceType::NONE;
}

void Board::update_castling_rights(const Move &move) {
    Piece piece = get_piece(move.from.row, move.from.col);

    if (piece.type == PieceType::KING) {
        if (piece.color == PieceColor::WHITE) {
            m_castling_rights.white_king_side = false;
            m_castling_rights.white_queen_side = false;
        } else {
            m_castling_rights.black_king_side = false;
            m_castling_rights.black_queen_side = false;
        }
    }

    if (piece.type == PieceType::ROOK) {
        if (piece.color == PieceColor::WHITE) {
            if (move.from.row == 7 && move.from.col == 0) {
                m_castling_rights.white_queen_side = false;
            } else if (move.from.row == 7 && move.from.col == 7) {
                m_castling_rights.white_king_side = false;
            }
        } else if (piece.color == PieceColor::BLACK) {
            if (move.from.row == 0 && move.from.col == 0) {
                m_castling_rights.black_queen_side = false;
            } else if (move.from.row == 0 && move.from.col == 7) {
                m_castling_rights.black_king_side = false;
            }
        }
    }

    if (move.captured_piece.type == PieceType::ROOK) {
        if (piece.color == PieceColor::BLACK) {
            if (move.capture_at.row == 7 && move.capture_at.col == 0) {
                m_castling_rights.white_queen_side = false;
            } else if (move.to.row == 7 && move.to.col == 7) {
                m_castling_rights.white_king_side = false;
            }
        } else if (piece.color == PieceColor::WHITE) {
            if (move.capture_at.row == 0 && move.capture_at.col == 0) {
                m_castling_rights.black_queen_side = false;
            } else if (move.capture_at.row == 0 && move.capture_at.col == 7) {
                m_castling_rights.black_king_side = false;
            }
        }
    }
}

void Board::update_en_passant_target(const Move &move) {
    clear_en_passant_target();

    if (move.moved_piece.type == PieceType::PAWN) {
        int amount_moved = move.from.row - move.to.row;

        if (abs(amount_moved) == 2) {
            int en_passant_row = (move.to.row + move.from.row) / 2;
            set_en_passant_target({en_passant_row, move.from.col});
        }
    }
}

void Board::clear_en_passant_target() {
    m_en_passant_target = {-1, -1};
}

void Board::set_en_passant_target(const Position &position) {
    m_en_passant_target = position;
}

void Board::save_state() {
    BoardState state;
    state.castling_rights = m_castling_rights;
    state.en_passant_target = m_en_passant_target;
    m_state_stack.push(state);
}

void Board::make_move(const Move &move) {
    save_state();
    update_en_passant_target(move);
    update_castling_rights(move);

    switch (move.type) {
        case MoveType::NORMAL: {
            set_piece(move.from.row, move.from.col, Piece{PieceType::NONE, PieceColor::NONE});
            set_piece(move.to.row, move.to.col, move.moved_piece);
            break;
        }
        case MoveType::CASTLE: {
            set_piece(move.from.row, move.from.col, Piece{PieceType::NONE, PieceColor::NONE});
            auto king = move.moved_piece;
            set_piece(move.to.row, move.to.col, king);
            bool is_kingside = (move.to.col > move.from.col);
            Position rook_from = is_kingside
                                     ? Position{move.from.row, 7}
                                     : Position{move.from.row, 0};
            Position rook_to = is_kingside
                                   ? Position{move.from.row, move.to.col - 1}
                                   : Position{move.from.row, move.to.col + 1};
            auto rook = get_piece(rook_from.row, rook_from.col);
            set_piece(rook_to.row, rook_to.col, rook);
            set_piece(rook_from.row, rook_from.col, Piece{PieceType::NONE, PieceColor::NONE});
            break;
        }
        case MoveType::EN_PASSANT: {
            set_piece(move.from.row, move.from.col, Piece{PieceType::NONE, PieceColor::NONE});
            set_piece(move.to.row, move.to.col, move.moved_piece);
            set_piece(move.capture_at.row, move.capture_at.col, Piece{PieceType::NONE, PieceColor::NONE});
            break;
        }
        case MoveType::PROMOTION: {
            set_piece(move.from.row, move.from.col, Piece{PieceType::NONE, PieceColor::NONE});
            set_piece(move.to.row, move.to.col, move.promoted_piece);
            break;
        }
        case MoveType::CAPTURE: {
            set_piece(move.from.row, move.from.col, Piece{PieceType::NONE, PieceColor::NONE});
            set_piece(move.to.row, move.to.col, move.moved_piece);
            break;
        }
        default: ;
    }
}

void Board::set_to_previous_state() {
    BoardState state = m_state_stack.top();
    m_castling_rights = state.castling_rights;
    m_en_passant_target = state.en_passant_target;
    m_state_stack.pop();
}

void Board::unmake_move(const Move &move) {
    switch (move.type) {
        case MoveType::NORMAL: {
            set_piece(move.to.row, move.to.col, Piece{PieceType::NONE, PieceColor::NONE});
            set_piece(move.from.row, move.from.col, move.moved_piece);
            break;
        }
        case MoveType::CASTLE: {
            auto king = move.moved_piece;
            set_piece(move.from.row, move.from.col, king);
            set_piece(move.to.row, move.to.col, Piece{PieceType::NONE, PieceColor::NONE});

            bool is_kingside = (move.to.col > move.from.col);
            Position rook_from = is_kingside
                                     ? Position{move.from.row, 7}
                                     : Position{move.from.row, 0};
            Position rook_to = is_kingside
                                   ? Position{move.from.row, move.to.col - 1}
                                   : Position{move.from.row, move.to.col + 1};

            auto rook = get_piece(rook_to.row, rook_to.col);
            set_piece(rook_from.row, rook_from.col, rook);
            set_piece(rook_to.row, rook_to.col, Piece{PieceType::NONE, PieceColor::NONE});
            break;
        }
        case MoveType::EN_PASSANT: {
            set_piece(move.from.row, move.from.col, move.moved_piece);
            set_piece(move.to.row, move.to.col, Piece{PieceType::NONE, PieceColor::NONE});
            set_piece(move.capture_at.row, move.capture_at.col, move.captured_piece);
            break;
        }
        case MoveType::PROMOTION: {
            set_piece(move.from.row, move.from.col, move.moved_piece);
            set_piece(move.to.row, move.to.col, move.captured_piece);
            break;
        }
        case MoveType::CAPTURE: {
            set_piece(move.from.row, move.from.col, move.moved_piece);
            set_piece(move.to.row, move.to.col, move.captured_piece);
            break;
        }
        default: ;
    }

    set_to_previous_state();
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

// TODO: Improve FEN handling
void Board::load_from_FEN(const std::string &FEN) {
    // Split the string by /
    int current_row = 0;
    int current_col = 0;
    for (auto it: FEN) {
        if (it == '/') {
            current_row++;
            current_col = 0;
            continue;
        }
        if (it == ' ') {
            break;
        }
        if (isdigit(it)) {
            int num_skips = it - '0';
            for (int col = 0; col < num_skips; col++) {
                set_piece(current_row, current_col + col, Piece{PieceType::NONE, PieceColor::NONE});
            }
            current_col += num_skips;
        } else {
            set_piece(current_row, current_col, piece_from_FEN_char(it));
            current_col += 1;
        }
    }
}

Position Board::get_en_passant_target() const {
    return m_en_passant_target;
}

bool Board::has_castling_rights_queen_side(const PieceColor &color) const {
    return color == PieceColor::WHITE ? m_castling_rights.white_queen_side : m_castling_rights.black_queen_side;
}

bool Board::has_castling_rights_king_side(const PieceColor &color) const {
    return color == PieceColor::WHITE ? m_castling_rights.white_king_side : m_castling_rights.black_king_side;
}

Position Board::get_king_position(PieceColor piece_color) const {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            Piece piece = get_piece(row, col);
            if (piece.type == PieceType::KING && piece.color == piece_color) {
                return Position{row, col};
            }
        }
    }

    return Position{-1, -1};
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
    return square_index.x >= 0 && square_index.x < BOARD_SIZE && square_index.y >= 0 && square_index.y <
           BOARD_SIZE;
}

bool is_empty_square(sf::Vector2i &square_index, const Board &board) {
    auto piece = board.get_piece(square_index);
    return piece.type == PieceType::NONE;
}
