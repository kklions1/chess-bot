#pragma once 

#include "enum.hpp"

#include <vector>
#include <set>
#include <string> 

extern int piece_mask;
extern int color_mask;

typedef struct Piece { 
    int data; 
    std::set<int> vision;

    int color();
    int type();

    std::string name(); 

    Piece(): data(PieceType::EMPTY), vision(std::set<int>()) {}
} Piece; 

typedef std::shared_ptr<Piece> Piece_ptr;