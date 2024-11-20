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