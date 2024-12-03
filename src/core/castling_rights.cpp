//
// Created by pknadimp on 12/2/24.
//

#include "castling_rights.h"

void reset_castling_rights(CastlingRights &rights) {
    rights.black_king_side = true;
    rights.white_king_side = true;
    rights.white_queen_side = true;
    rights.black_queen_side = true;
}