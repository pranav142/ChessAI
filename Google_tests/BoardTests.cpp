//
// Created by pknadimp on 11/20/24.
//

#include <gtest/gtest.h>
#include "Board.h"

TEST(BoardIsEmptyTest, HandlesOutOfBoundsInput) {
    Board board;
    board.initialize();

    EXPECT_TRUE(board.is_empty(-1, -1)) << "Square (-1, -1) is out of bounds";
    EXPECT_TRUE(board.is_empty(9, 9)) << "Square (9, 9) is out of bounds";
}

TEST(BoardIsEmptyTest, HandlesSquaresWithPieces) {
    Board board;
    board.initialize();

    EXPECT_FALSE(board.is_empty(1, 0)) << "Square (1, 0) should not be empty (should contain a pawn)";
    EXPECT_FALSE(board.is_empty(0, 0)) << "Square (0, 0) should not be empty (should contain a rook)";
}

TEST(BoardIsEmptyTest, HandlesEmptySquares) {
    Board board;
    board.initialize();

    EXPECT_TRUE(board.is_empty(2, 0)) << "Square (2, 0) should be empty";
    EXPECT_TRUE(board.is_empty(4, 4)) << "Square (4, 4) should be empty";
}

TEST(BoardLoadFromFenTest, HandlesValidFen) {
    Board board;

    // Example FEN for a standard starting position
    std::string example_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FEN fen;
    create_fen_from_string(example_fen, fen);
    board.load_from_FEN(fen);

    auto piece = board.get_piece(0, 0);
    EXPECT_EQ(piece.type, PieceType::ROOK) << "Square (0, 0) should be a black rook";
    EXPECT_EQ(piece.color, PieceColor::BLACK) << "Square (0, 0) should be a black rook";

    piece = board.get_piece(6, 4);
    EXPECT_EQ(piece.type, PieceType::PAWN) << "Square (6, 4) should be a white pawn";
    EXPECT_EQ(piece.color, PieceColor::WHITE) << "Square (6, 4) should be a white pawn";

    piece = board.get_piece(4, 4);
    EXPECT_EQ(piece.type, PieceType::NONE) << "Square (4, 4) should be empty";
    EXPECT_EQ(piece.color, PieceColor::NONE) << "Square (4, 4) should be empty";

    piece = board.get_piece(7, 4);
    EXPECT_EQ(piece.type, PieceType::KING) << "Square (7, 4) should be a white king";
    EXPECT_EQ(piece.color, PieceColor::WHITE) << "Square (7, 4) should be a white king";

    piece = board.get_piece(0, 1);
    EXPECT_EQ(piece.type, PieceType::KNIGHT) << "Square (0, 1) should be a black knight";
    EXPECT_EQ(piece.color, PieceColor::BLACK) << "Square (0, 1) should be a black knight";
}