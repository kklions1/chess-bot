#pragma once 

#include "../common/common_structs.hpp"
#include "board.hpp"

void engine_main(Board&);
std::vector<Move> generate_legal_moves(const Board&);

void generate_bishop_moves(int, std::vector<Move>&);
void generate_pawn_moves(int, std::vector<Move>&);
void generate_bishop_moves(int, std::vector<Move>&);
void generate_horse_moves(int, std::vector<Move>&);
void generate_rook_moves(int, std::vector<Move>&);
void generate_queen_moves(int, std::vector<Move>&);
void generate_king_moves(int, std::vector<Move>&);

