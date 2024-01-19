#pragma once 

#include "piece.hpp"
#include "enum.hpp"

#include <memory> 

typedef struct Board { 
    Piece_ptr board[64];

    bool white_castle_short;
    bool white_castle_long;
    bool black_castle_short;
    bool black_castle_long;

    int en_passant_target; 
    int halfmove_clock; 
    int fullmove_clock; 

    int active_color;

    MoveResult move_piece(int, int);
    void prune_illegal_moves(Piece_ptr, int);
    void cast_ray(Piece_ptr, int, int);
    
    // Pseudo-legal moves to start. 
    void generate_legal_moves();

    Board();

    void pawn_moves(Piece_ptr, int);
    void rook_moves(Piece_ptr, int);
    void bishop_moves(Piece_ptr, int);
    void horsy_moves(Piece_ptr, int);
    void queen_moves(Piece_ptr, int);
    void king_vision(Piece_ptr, int);
} Board; 

typedef struct Move { 
    int piece;
    int starting_index;
    int new_index;
} Move; 