#pragma once 

#include "enum.hpp"

#include <vector>
#include <set>
#include <string> 
#include <memory>

typedef struct Piece { 
    int data; 
    std::set<int> vision;

    int color();
    int type();
    bool is_empty(); 

    std::string name(); 
    std::shared_ptr<Piece> clone();

    static std::shared_ptr<Piece> make_empty(); 

    explicit Piece();

private: 
    explicit Piece(int);
} Piece; 

std::string colorName(int);
void move(Piece*, Piece*);