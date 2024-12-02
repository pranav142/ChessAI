//
// Created by pknadimp on 12/1/24.
//

#include "perft.h"

unsigned long perft(Game& game, int depth) {
    if (depth == 0) {
        return 1;
    }

    unsigned long result = 0;
    auto moves = game.get_all_legal_moves();

    for (auto &move : moves) {
        game.make_move(move);
        result += perft(game, depth - 1);
        game.unmake_move(move);
    }

    return result;
}
