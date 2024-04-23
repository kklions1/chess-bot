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
    void addAttack(int); 

    std::string name(); 
    std::shared_ptr<Piece> clone();

    static std::shared_ptr<Piece> make_empty(); 

    explicit Piece();

private: 
    explicit Piece(int);
} Piece; 

typedef std::shared_ptr<Piece> Piece_ptr;

std::string colorName(int);
