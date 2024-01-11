#pragma once 

#include "piece.hpp"
#include "board.hpp"

typedef struct Move { 
    int piece;
    int starting_index;
    int new_index;
} Move; 