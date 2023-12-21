#pragma once 

#include "../common/common_structs.hpp"
#include "board.hpp"

std::vector<Move> generate_legal_moves(const Board&);
void generate_pawn_moves(int, std::vector<Move>&);
