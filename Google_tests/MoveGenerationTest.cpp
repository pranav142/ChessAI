//
// Created by pknadimp on 11/27/24.
//


#include <Board.h>
#include <gtest/gtest.h>

TEST(GenerateMoves, HandlesCastleMoves) {
    Board board;
    board.load_from_FEN("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
}

TEST(GenerateMoves, HandlesEnPassant) {

}

TEST(GenerateMoves, HandlesPieceCapture) {

}

TEST(GenerateMoves, TestCheckMoves) {

}

TEST(GenerateMoves, HandlesPawmPromotion) {

}



