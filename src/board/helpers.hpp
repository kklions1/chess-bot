#pragma once 

#include <utility> 
#include <string> 
#include <vector>
#include <unordered_map>

#include "board.hpp" 
#include "piece.hpp" 

extern std::set<int> left_edges; // A File 
extern std::set<int> right_edges; // H File 
extern std::set<int> top_edges; // 8th Rank
extern std::set<int> bottom_edges; // 1st Rank
extern std::set<int> edges;

class Board;

// simple struct for containing castling rights
typedef struct CastlingRights { 
    bool white_castle_long;
    bool white_castle_short;
    bool black_castle_long;
    bool black_castle_short;

    CastlingRights(bool white_castle_long, bool white_castle_short, bool black_castle_long, bool black_castle_short) : 
        white_castle_long(white_castle_long),
        white_castle_short(white_castle_short),
        black_castle_long(black_castle_long),
        black_castle_short(black_castle_short) {} 
} CastlingRights; 

// console printing - usually for debugging/planning
void print_board(const Board&); 
void print_index();

// string parsing too complex to leave in the main factory function
// but not complex enough for me to put it in its own file 
int parse_active_color(const std::string&);
CastlingRights parse_castling_rights(const std::string&);
std::array<std::shared_ptr<Piece>, 64> parse_piece_locations(const std::string& fen); 
int get_board_index(const char*);
std::vector<std::string> split(std::string, const char);

// literal edge case detection 
bool is_edge_index(int);
bool is_edge_in_direction(int, int);


