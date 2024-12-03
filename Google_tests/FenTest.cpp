//
// Created by pknadimp on 12/2/24.
//

#include <gtest/gtest.h>
#include "FEN.h" // Assuming your FEN structure and create_fen_from_string function are in FEN.h

TEST(FENParserTest, ValidFEN) {
    FEN fen;
    std::string valid_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    EXPECT_TRUE(create_fen_from_string(valid_fen, fen));

    EXPECT_EQ(fen.board_string, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    EXPECT_EQ(fen.side_to_move, PieceColor::WHITE);
    EXPECT_TRUE(fen.castling_rights.white_king_side);
    EXPECT_TRUE(fen.castling_rights.white_queen_side);
    EXPECT_TRUE(fen.castling_rights.black_king_side);
    EXPECT_TRUE(fen.castling_rights.black_queen_side);
    EXPECT_EQ(fen.en_passant_target.row, -1);
    EXPECT_EQ(fen.en_passant_target.col, -1);
    EXPECT_EQ(fen.half_move_clock, 0);
    EXPECT_EQ(fen.full_move_number, 1);
}

TEST(FENParserTest, FENWithNoCastlingRights) {
    FEN fen;
    std::string fen_no_castling = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";

    EXPECT_TRUE(create_fen_from_string(fen_no_castling, fen));

    EXPECT_EQ(fen.board_string, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    EXPECT_EQ(fen.side_to_move, PieceColor::WHITE);
    EXPECT_FALSE(fen.castling_rights.white_king_side);
    EXPECT_FALSE(fen.castling_rights.white_queen_side);
    EXPECT_FALSE(fen.castling_rights.black_king_side);
    EXPECT_FALSE(fen.castling_rights.black_queen_side);
    EXPECT_EQ(fen.en_passant_target.row, -1);
    EXPECT_EQ(fen.en_passant_target.col, -1);
    EXPECT_EQ(fen.half_move_clock, 0);
    EXPECT_EQ(fen.full_move_number, 1);
}

TEST(FENParserTest, FENWithEnPassantTarget) {
    FEN fen;
    std::string fen_with_en_passant = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";

    EXPECT_TRUE(create_fen_from_string(fen_with_en_passant, fen));

    EXPECT_EQ(fen.board_string, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR");
    EXPECT_EQ(fen.side_to_move, PieceColor::BLACK);
    EXPECT_TRUE(fen.castling_rights.white_king_side);
    EXPECT_TRUE(fen.castling_rights.white_queen_side);
    EXPECT_TRUE(fen.castling_rights.black_king_side);
    EXPECT_TRUE(fen.castling_rights.black_queen_side);
    EXPECT_EQ(fen.en_passant_target.row, 2); // 'e3' => row 2
    EXPECT_EQ(fen.en_passant_target.col, 4); // 'e3' => col 4
    EXPECT_EQ(fen.half_move_clock, 0);
    EXPECT_EQ(fen.full_move_number, 1);
}

TEST(FENParserTest, InvalidFEN_InvalidSideToMove) {
    FEN fen;
    std::string invalid_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR x KQkq - 0 1";

    EXPECT_FALSE(create_fen_from_string(invalid_fen, fen));
}

TEST(FENParserTest, InvalidFEN_InvalidEnPassant) {
    FEN fen;
    std::string invalid_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq x9 0 1";

    EXPECT_FALSE(create_fen_from_string(invalid_fen, fen));
}

TEST(FENParserTest, InvalidFEN_MissingSegments) {
    FEN fen;
    std::string incomplete_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    EXPECT_FALSE(create_fen_from_string(incomplete_fen, fen));
}
