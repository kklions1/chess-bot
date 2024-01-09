#pragma once 

#include "piece_type.hpp"

#include <vector>

typedef struct Piece { 
    int type; 
    std::vector<int> vision; 

    void (*calc_vision)(Piece&, int); 

    Piece(): type(0), vision(std::vector<int>()), calc_vision() {}
    Piece(int type, void (*vision)(Piece&, int)): type(type), calc_vision(vision) {} 
} Piece; 

void calculate_pawn_vision(Piece&, int);

