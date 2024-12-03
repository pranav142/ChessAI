//
// Created by pknadimp on 12/1/24.
//

#include <perft.h>
#include <gtest/gtest.h>

TEST(Perft, InitialPosition) {
    Game game;
    game.initialize();
    game.set_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    EXPECT_EQ(perft(game, 0), 1);
    EXPECT_EQ(perft(game, 1), 20);
    EXPECT_EQ(perft(game, 2), 400);
    EXPECT_EQ(perft(game, 3), 8902);
    EXPECT_EQ(perft(game, 4), 197281);
}

TEST(Perft, TrickyPosition4) {
    Game game;
    game.initialize();

    game.set_FEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    EXPECT_EQ(perft(game, 0), 1);
    EXPECT_EQ(perft(game, 1), 6);
    EXPECT_EQ(perft(game, 2), 264);
    EXPECT_EQ(perft(game, 3), 9467);
    EXPECT_EQ(perft(game, 4), 422333);
}

TEST(Perft, TrickyPosition5) {
    Game game;
    game.initialize();
    // TODO: FIX fen initialization to handle rest of stuff!
    game.set_FEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

    EXPECT_EQ(perft(game, 0), 1);
    EXPECT_EQ(perft(game, 1), 44);
    EXPECT_EQ(perft(game, 2), 1486);
    EXPECT_EQ(perft(game, 3), 62379);
}


