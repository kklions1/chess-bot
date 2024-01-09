#pragma once 

#include "common_structs.hpp"

extern int piece_mask;
extern int color_mask;

bool is_white(int);
bool is_black(int); 
bool is_pawn(int);
bool is_horse(int);
bool is_rook(int);
bool is_bishop(int);
bool is_queen(int);
bool is_king(int);