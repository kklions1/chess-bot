#pragma once 

#include "piece.hpp"

typedef unsigned long long Bitboard; 

typedef struct Board { 
    Piece board[64]; 

    bool white_castle_short; 
    bool white_castle_long; 
    bool black_castle_short;
    bool black_castle_long; 

    int en_passant_target; 
    int halfmove_clock; 
    int fullmove_clock; 

    int active_color;

    Board() :
        board { Piece() },
        white_castle_short(false), white_castle_long(false),
        black_castle_short(false), black_castle_long(false), 
        en_passant_target(-1), halfmove_clock(0), fullmove_clock(0),
        active_color(-1) {}
} Board; 

typedef struct Move { 
    int piece;
    int starting_index;
    int new_index;
} Move; 