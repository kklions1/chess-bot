#pragma once 

#include "enum.hpp"

#include <vector>
#include <set>
#include <string> 

extern int piece_mask;
extern int color_mask;

extern std::set<int> left_edges; // A File 
extern std::set<int> right_edges; // H File 
extern std::set<int> top_edges; // 8th Rank
extern std::set<int> bottom_edges; // 1st Rank

typedef struct Piece { 
    int data; 
    std::set<int> vision; 

    void (*calc_vision)(Piece&,  int); 

    int color();
    int type();

    std::string name(); 

    Piece(): data(PieceType::EMPTY), vision(std::set<int>()), calc_vision([](Piece& self, int index) { /* no-op */ }) {}
} Piece; 

typedef std::shared_ptr<Piece> Piece_ptr;

bool is_edge_index(int);
bool is_edge_in_direction(int, int);