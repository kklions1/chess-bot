#pragma once 

#include "piece.hpp"

#include <memory>

typedef struct Board { 
    std::shared_ptr<Piece> board[64];

    bool white_castle_short;
    bool white_castle_long;
    bool black_castle_short;
    bool black_castle_long;

    int en_passant_target; 
    int halfmove_clock; 
    int fullmove_clock; 

    int active_color;

    Board();
} Board; 

typedef struct Move { 
    int piece;
    int starting_index;
    int new_index;
} Move; 