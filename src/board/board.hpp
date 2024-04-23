#pragma once 

#include <string> 
#include <ctype.h>
#include <iostream> 
#include <vector>
#include <memory> 
#include <utility> 

#include "piece.hpp" 
#include "enum.hpp"
#include "coordinate.hpp"

extern std::set<int> left_edges; // A File 
extern std::set<int> right_edges; // H File 
extern std::set<int> top_edges; // 8th Rank
extern std::set<int> bottom_edges; // 1st Rank

typedef struct Board { 
    std::shared_ptr<Piece> board[64];

    bool white_castle_short;
    bool white_castle_long;
    bool black_castle_short;
    bool black_castle_long;

    int en_passant_target; 
    int halfmove_clock; 
    int fullmove_clock; 

    int active_color;

    MoveType move_piece(int, int);
    void cast_ray(Piece*, int, int);
    void add_attack(Piece*, int);
    
    // Pseudo-legal moves to start. 
    void generate_legal_moves();

    Board();

    std::shared_ptr<Board> clone();
    std::shared_ptr<Board> next_position(int, int);

    bool active_player_in_check();
    bool nap_in_check(); 
    int get_ap_king_index(); 

    void pawn_moves(Piece*, int);
    void rook_moves(Piece*, int);
    void bishop_moves(Piece*, int);
    void horsy_moves(Piece*, int);
    void queen_moves(Piece*, int);
    void king_vision(Piece*, int);

private: 
    void increment_clock();
    int find_king_index_by_color(int);
    void prune_illegal_moves();
    void calc_piece_vision(); 
    std::shared_ptr<Board> peek_next_position(int, int); 

} Board; 

void print_board(const Board&); 
void parse_fen_string(std::string, Board&); 
std::string generate_fen_string(const Board&);
int get_board_index(const char*);
std::vector<std::string> split(std::string, const char);
void print_index();

bool is_edge_index(int);
bool is_edge_in_direction(int, int);
