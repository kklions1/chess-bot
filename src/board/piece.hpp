#pragma once 

#include "enum.hpp"

#include <vector>
#include <set>
#include <string> 
#include <memory>

extern int piece_mask;
extern int color_mask;

typedef struct Piece { 
    int data; 
    std::set<int> vision;

    int color();
    int type();

    std::string name(); 

    static std::shared_ptr<Piece> make_empty(); 
    static std::shared_ptr<Piece> make_white_pawn();

    explicit Piece();

private: 
    explicit Piece(int);
} Piece; 

typedef std::shared_ptr<Piece> Piece_ptr;