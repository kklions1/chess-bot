#pragma once 

#include <string> 

#include "../common/common_structs.hpp" 

void print_board(const Board&); 
void parse_fen_string(const std::string&, Board&); 
std::string generate_fen_string(const Board&);
int get_board_index(const char*);