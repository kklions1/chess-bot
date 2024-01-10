#pragma once 

#include <vector>

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
    int type; 
    std::vector<int> vision; 

    void (*calc_vision)(Piece&, int); 

    Piece(): type(0), vision(std::vector<int>()), calc_vision([](Piece& self, int index) { /* no-op */ }) {}
    Piece(int type, void (*vision)(Piece&, int)): type(type), calc_vision(vision) {} 
} Piece; 

void calculate_pawn_vision(Piece&, int);

