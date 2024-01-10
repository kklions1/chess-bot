#pragma once 

#include <vector>

extern int piece_mask;
extern int color_mask;

typedef enum PieceType { 
    EMPTY = 0, 
    PAWN = 1, 
    KNIGHT = 2, 
    BISHOP  = 3, 
    ROOK = 4, 
    QUEEN = 5, 
    KING = 6, 

    WHITE = 8,
    BLACK = 16 
} PieceType; 

typedef struct Piece { 
    int data; 
    std::vector<int> vision; 

    void (*calc_vision)(Piece&, int); 

    int color();
    int type();

    Piece(): data(PieceType::EMPTY), vision(std::vector<int>()), calc_vision([](Piece& self, int index) { /* no-op */ }) {}
} Piece; 

void calculate_pawn_vision(Piece&, int);

