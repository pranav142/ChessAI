//
// Created by pknadimp on 11/19/24.
//

#include "move_generation.h"


bool is_promotion_move(const Piece &piece, int to_row) {
    if (piece.type != PieceType::PAWN) {
        return false;
    }
    int end_row = (piece.color == PieceColor::WHITE) ? 0 : 7;
    return to_row == end_row;
}


std::vector<Move> generate_pawn_moves(const Board &board, const Position position, const Piece &piece) {
    std::vector<Move> available_moves;
    int direction = (piece.color == PieceColor::WHITE) ? -1 : 1;
    int start_row = (piece.color == PieceColor::WHITE) ? 6 : 1;

    Position forward_pos{position.row + direction, position.col};
    if (board.is_empty(forward_pos.row, forward_pos.col)) {
        if (is_promotion_move(piece, forward_pos.row)) {
            Piece empty_piece = {PieceType::NONE, PieceColor::NONE};
            available_moves.push_back(create_promotion_move(position, forward_pos, piece, empty_piece,
                                                            Piece{PieceType::QUEEN, piece.color}));
            available_moves.push_back(create_promotion_move(position, forward_pos, piece, empty_piece,
                                                            Piece{PieceType::BISHOP, piece.color}));
            available_moves.push_back(create_promotion_move(position, forward_pos, piece, empty_piece,
                                                            Piece{PieceType::ROOK, piece.color}));
            available_moves.push_back(create_promotion_move(position, forward_pos, piece, empty_piece,
                                                            Piece{PieceType::KNIGHT, piece.color}));
        } else {
            Move forward = create_normal_move(position, forward_pos, piece);
            available_moves.push_back(forward);
        }

        Position double_forward_pos{position.row + 2 * direction, position.col};
        if (position.row == start_row && board.is_empty(double_forward_pos.row, double_forward_pos.col)) {
            Move double_forward = create_normal_move(position, double_forward_pos, piece);
            available_moves.push_back(double_forward);
        }
    }

    Position capture_left{position.row + direction, position.col - 1};
    Position capture_right{position.row + direction, position.col + 1};

    if (!board.is_empty(capture_left.row, capture_left.col) &&
        board.get_piece(capture_left.row, capture_left.col).color != piece.color) {
        if (is_promotion_move(piece, capture_left.row)) {
            auto captured_piece = board.get_piece(capture_left.row, capture_left.col);
            available_moves.push_back(create_promotion_move(position, capture_left, piece, captured_piece,
                                                            Piece{PieceType::QUEEN, piece.color}));
            available_moves.push_back(create_promotion_move(position, capture_left, piece, captured_piece,
                                                            Piece{PieceType::BISHOP, piece.color}));
            available_moves.push_back(create_promotion_move(position, capture_left, piece, captured_piece,
                                                            Piece{PieceType::ROOK, piece.color}));
            available_moves.push_back(create_promotion_move(position, capture_left, piece, captured_piece,
                                                            Piece{PieceType::KNIGHT, piece.color}));
        } else {
            Move capture_left_move = create_capture_move(position, capture_left, piece,
                                                         board.get_piece(capture_left.row, capture_left.col));
            available_moves.push_back(capture_left_move);
        }
    }

    if (!board.is_empty(capture_right.row, capture_right.col) && board.get_piece(capture_right.row, capture_right.col).
        color != piece.color) {
        if (is_promotion_move(piece, capture_right.row)) {
            auto captured_piece = board.get_piece(capture_right.row, capture_right.col);
            available_moves.push_back(create_promotion_move(position, capture_right, piece, captured_piece,
                                                            Piece{PieceType::QUEEN, piece.color}));
            available_moves.push_back(create_promotion_move(position, capture_right, piece, captured_piece,
                                                            Piece{PieceType::BISHOP, piece.color}));
            available_moves.push_back(create_promotion_move(position, capture_right, piece, captured_piece,
                                                            Piece{PieceType::ROOK, piece.color}));
            available_moves.push_back(create_promotion_move(position, capture_right, piece, captured_piece,
                                                            Piece{PieceType::KNIGHT, piece.color}));
        } else {
            Move capture_right_move = create_capture_move(position, capture_right, piece,
                                                          board.get_piece(capture_right.row, capture_right.col));
            available_moves.push_back(capture_right_move);
        }
    }

    Position en_passant_target = board.get_en_passant_target();
    if (en_passant_target.row > -1 && en_passant_target.col > -1) {
        if (en_passant_target.row == position.row + direction) {
            if (en_passant_target.col == position.col - 1 || en_passant_target.col == position.col + 1) {
                Position captured_position = Position{position.row, en_passant_target.col};
                Piece captured_piece = board.get_piece(position.row, en_passant_target.col);
                available_moves.push_back(
                    create_en_passant_move(position, en_passant_target, piece, captured_piece, captured_position));
            }
        }
    }

    return available_moves;
}

std::vector<Move> generate_pawn_attacking_moves(const Board &board, const Position position, const Piece &piece) {
    std::vector<Move> available_moves;
    int direction = (piece.color == PieceColor::WHITE) ? -1 : 1;

    Position capture_left{position.row + direction, position.col - 1};
    Position capture_right{position.row + direction, position.col + 1};

    if (!board.is_empty(capture_left.row, capture_left.col) &&
        board.get_piece(capture_left.row, capture_left.col).color != piece.color || board.is_empty(
            capture_left.row, capture_left.col)) {
        Move capture_left_move = create_capture_move(position, capture_left, piece,
                                                     board.get_piece(capture_left.row, capture_left.col));
        available_moves.push_back(capture_left_move);
    }

    if (!board.is_empty(capture_right.row, capture_right.col) && board.get_piece(capture_right.row, capture_right.col).
        color != piece.color || board.is_empty(capture_right.row, capture_right.col)) {
        Move capture_right_move = create_capture_move(position, capture_right, piece,
                                                      board.get_piece(capture_right.row, capture_right.col));
        available_moves.push_back(capture_right_move);
    }

    // check if can promote
    // check if can en passant

    return available_moves;
}


std::vector<Move> generate_bishop_moves(const Board &board, const Position position, const Piece &piece) {
    std::vector<Move> available_moves;

    // Define the four diagonal directions: top-right, top-left, bottom-right, and bottom-left
    std::vector<std::pair<int, int> > directions = {
        {1, 1}, // top-right
        {1, -1}, // top-left
        {-1, 1}, // bottom-right
        {-1, -1} // bottom-left
    };

    for (const auto &[row_delta, col_delta]: directions) {
        int row = position.row + row_delta;
        int col = position.col + col_delta;

        while (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            Position new_pos{row, col};

            if (!board.is_empty(new_pos.row, new_pos.col)) {
                if (board.get_piece(new_pos.row, new_pos.col).color != piece.color) {
                    Move capture_move = create_capture_move(position, new_pos, piece,
                                                            board.get_piece(new_pos.row, new_pos.col));
                    available_moves.push_back(capture_move);
                }
                break;
            }

            Move move = create_normal_move(position, new_pos, piece);
            available_moves.push_back(move);

            row += row_delta;
            col += col_delta;
        }
    }

    return available_moves;
}

std::vector<Move> generate_rook_moves(const Board &board, const Position position, const Piece &piece) {
    std::vector<Move> available_moves;

    std::vector<std::pair<int, int> > directions = {
        {0, 1}, // right
        {0, -1}, // left
        {1, 0}, // down
        {-1, 0} // up
    };

    for (const auto &[row_delta, col_delta]: directions) {
        int row = position.row + row_delta;
        int col = position.col + col_delta;

        while (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            Position new_pos{row, col};


            if (!board.is_empty(new_pos.row, new_pos.col)) {
                if (board.get_piece(new_pos.row, new_pos.col).color != piece.color) {
                    Move capture_move = create_capture_move(position, new_pos, piece,
                                                            board.get_piece(new_pos.row, new_pos.col));
                    available_moves.push_back(capture_move);
                }
                break;
            }

            Move move = create_normal_move(position, new_pos, piece);
            available_moves.push_back(move);

            row += row_delta;
            col += col_delta;
        }
    }

    return available_moves;
}


std::vector<Move> generate_knight_moves(const Board &board, const Position position, const Piece &piece) {
    std::vector<Move> available_moves;

    // Define all 8 possible moves for a knight
    std::vector<std::pair<int, int> > moves = {
        {2, 1}, // move two rows down, one column right
        {2, -1}, // move two rows down, one column left
        {-2, 1}, // move two rows up, one column right
        {-2, -1}, // move two rows up, one column left
        {1, 2}, // move one row down, two columns right
        {1, -2}, // move one row down, two columns left
        {-1, 2}, // move one row up, two columns right
        {-1, -2} // move one row up, two columns left
    };

    for (const auto &[row_delta, col_delta]: moves) {
        int row = position.row + row_delta;
        int col = position.col + col_delta;

        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            Position new_pos{row, col};

            if (board.is_empty(new_pos.row, new_pos.col)) {
                Move move = create_normal_move(position, new_pos, piece);
                available_moves.push_back(move);
            } else if (board.get_piece(new_pos.row, new_pos.col).color != piece.
                       color) {
                Move capture_move = create_capture_move(position, new_pos, piece,
                                                        board.get_piece(new_pos.row, new_pos.col));
                available_moves.push_back(capture_move);
            }
        }
    }

    return available_moves;
}

std::vector<Move> generate_queen_moves(const Board &board, const Position position, const Piece &piece) {
    std::vector<Move> available_moves;

    std::vector<std::pair<int, int> > directions = {
        {1, 0}, // down
        {-1, 0}, // up
        {0, 1}, // right
        {0, -1}, // left
        {1, 1}, // down-right (diagonal)
        {1, -1}, // down-left (diagonal)
        {-1, 1}, // up-right (diagonal)
        {-1, -1} // up-left (diagonal)
    };

    for (const auto &[row_delta, col_delta]: directions) {
        int row = position.row + row_delta;
        int col = position.col + col_delta;

        while (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            Position new_pos{row, col};

            if (!board.is_empty(new_pos.row, new_pos.col)) {
                if (board.get_piece(new_pos.row, new_pos.col).color != piece.color) {
                    Move capture_move = create_capture_move(position, new_pos, piece,
                                                            board.get_piece(new_pos.row, new_pos.col));
                    available_moves.push_back(capture_move);
                }
                break;
            }

            Move move = create_normal_move(position, new_pos, piece);
            available_moves.push_back(move);

            row += row_delta;
            col += col_delta;
        }
    }

    return available_moves;
}

bool can_castle_kingside(const Piece &king, const Position &king_position, const Board &board) {
    if (king.type != PieceType::KING) {
        return false;
    }

    if (!board.has_castling_rights_king_side(king.color)) {
        return false;
    }

    for (int col = king_position.col + 1; col < BOARD_SIZE - 1; col++) {
        if (!board.is_empty(king_position.row, col)) {
            return false;
        }
    }

    for (int col = king_position.col; col <= king_position.col + 2; col++) {
        if (is_square_attacked_by_color(king_position.row, col, get_enemy_color(king.color), board)) {
            return false;
        }
    }

    return true;
}

bool can_castle_queenside(const Piece &king, const Position &king_position, const Board &board) {
    if (king.type != PieceType::KING) {
        return false;
    }

    if (!board.has_castling_rights_queen_side(king.color)) {
        return false;
    }

    for (int col = 1; col < king_position.col; col++) {
        if (!board.is_empty(king_position.row, col)) {
            return false;
        }
    }

    for (int col = king_position.col - 1; col >= king_position.col - 3; col--) {
        if (is_square_attacked_by_color(king_position.row, col, get_enemy_color(king.color), board)) {
            return false;
        }
    }


    return true;
}

std::vector<Move> generate_king_attacking_moves(const Board &board, const Position position, const Piece &piece) {
    std::vector<Move> available_moves;

    std::vector<std::pair<int, int> > moves = {
        {1, 0}, // down
        {-1, 0}, // up
        {0, 1}, // right
        {0, -1}, // left
        {1, 1}, // down-right (diagonal)
        {1, -1}, // down-left (diagonal)
        {-1, 1}, // up-right (diagonal)
        {-1, -1} // up-left (diagonal)
    };

    for (const auto &[row_delta, col_delta]: moves) {
        int row = position.row + row_delta;
        int col = position.col + col_delta;

        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            Position new_pos{row, col};

            if (board.is_empty(new_pos.row, new_pos.col)) {
                Move move = create_normal_move(position, new_pos, piece);
                available_moves.push_back(move);
            } else if (board.get_piece(new_pos.row, new_pos.col).color != piece.
                       color) {
                Move attack_move = create_capture_move(position, new_pos, piece,
                                                       board.get_piece(new_pos.row, new_pos.col));
                available_moves.push_back(attack_move);
            }
        }
    }

    return available_moves;
}


std::vector<Move> generate_king_moves(const Board &board, const Position position, const Piece &piece) {
    std::vector<Move> available_moves;

    std::vector<std::pair<int, int> > moves = {
        {1, 0}, // down
        {-1, 0}, // up
        {0, 1}, // right
        {0, -1}, // left
        {1, 1}, // down-right (diagonal)
        {1, -1}, // down-left (diagonal)
        {-1, 1}, // up-right (diagonal)
        {-1, -1} // up-left (diagonal)
    };

    for (const auto &[row_delta, col_delta]: moves) {
        int row = position.row + row_delta;
        int col = position.col + col_delta;

        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            Position new_pos{row, col};

            if (is_square_attacked_by_color(row, col, get_enemy_color(piece.color), board)) {
                continue;
            }

            if (board.is_empty(new_pos.row, new_pos.col)) {
                Move move = create_normal_move(position, new_pos, piece);
                available_moves.push_back(move);
            } else if (board.get_piece(new_pos.row, new_pos.col).color != piece.
                       color) {
                Move attack_move = create_capture_move(position, new_pos, piece,
                                                       board.get_piece(new_pos.row, new_pos.col));
                available_moves.push_back(attack_move);
            }
        }
    }

    if (can_castle_kingside(piece, position, board)) {
        Position castle_position{position.row, position.col + 2};
        Move castle_move = create_castle_move(position, castle_position, piece);
        available_moves.push_back(castle_move);
    }

    if (can_castle_queenside(piece, position, board)) {
        Position castle_position{position.row, position.col - 3};
        Move castle_move = create_castle_move(position, castle_position, piece);
        available_moves.push_back(castle_move);
    }

    return available_moves;
}


// Generate attacking moves() // seperate function
std::vector<Move> generate_attacking_moves(const Piece &piece, const Board &board, Position position) {
    switch (piece.type) {
        case PieceType::PAWN:
            // this needs to be adjusted
            return generate_pawn_attacking_moves(board, position, piece);
        case PieceType::BISHOP:
            return generate_bishop_moves(board, position, piece);
        case PieceType::ROOK:
            return generate_rook_moves(board, position, piece);
        case PieceType::KNIGHT:
            return generate_knight_moves(board, position, piece);
        case PieceType::QUEEN:
            return generate_queen_moves(board, position, piece);
        case PieceType::KING:
            return generate_king_attacking_moves(board, position, piece);
        default: ;
    }

    return {};
}

// Generating Moves is SUS
std::vector<Move> generate_moves(const Piece &piece, Board &board, Position position) {
    std::vector <Move> moves;

    switch (piece.type) {
        case PieceType::PAWN:
            moves = generate_pawn_moves(board, position, piece); break;
        case PieceType::BISHOP:
            moves = generate_bishop_moves(board, position, piece); break;
        case PieceType::ROOK:
            moves = generate_rook_moves(board, position, piece); break;
        case PieceType::KNIGHT:
            moves = generate_knight_moves(board, position, piece); break;
        case PieceType::QUEEN:
            moves = generate_queen_moves(board, position, piece); break;
        case PieceType::KING:
            moves = generate_king_moves(board, position, piece); break;
        default: ;
    }

    std::vector <Move> legal_moves;
    for (auto &move : moves) {
        if (is_king_safe_after_move(board, move, piece.color)) {
            legal_moves.push_back(move);
        }
    }

    return legal_moves;
}

bool is_king_safe_after_move(Board& board, const Move& move, PieceColor piece_color) {
    bool is_king_safe = true;
    board.make_move(move);
    Position king_posiiton = board.get_king_position(piece_color);
    if (is_square_attacked_by_color(king_posiiton.row, king_posiiton.col, get_enemy_color(piece_color), board)) {
        is_king_safe = false;
    }
    board.unmake_move(move);
    return is_king_safe;
}

bool is_square_attacked_by_color(int row, int col, const PieceColor &color, const Board &board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            auto piece = board.get_piece(i, j);
            if (piece.color == color && is_piece_attacking_square(piece, Position{i, j}, Position{row, col}, board)) {
                return true;
            }
        }
    }
    return false;
}

// There is a bug that is occuring due to recursive call to generate moves
bool is_piece_attacking_square(const Piece &piece, Position piece_position, Position target, const Board &board) {
    const auto moves = generate_attacking_moves(piece, board, piece_position);
    for (auto move: moves) {
        if (move.to.row == target.row && move.to.col == target.col) {
            return true;
        }
    }

    return false;
}
