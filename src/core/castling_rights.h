//
// Created by pknadimp on 12/2/24.
//

#ifndef CASTLING_RIGHTS_H
#define CASTLING_RIGHTS_H


// Simply stores right to castle not the ability to castle
struct CastlingRights {
    bool white_king_side;
    bool white_queen_side;
    bool black_king_side;
    bool black_queen_side;
};

void reset_castling_rights(CastlingRights &rights);


#endif //CASTLING_RIGHTS_H
