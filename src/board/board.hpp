#pragma once 

#include "../common/common_structs.hpp" 

#include <string> 
#include <ctype.h>
#include <iostream> 
#include <vector>

void print_board(const Board&); 
void parse_fen_string(std::string, Board&); 
std::string generate_fen_string(const Board&);
int get_board_index(const char*);
std::vector<std::string> split(std::string, const char);