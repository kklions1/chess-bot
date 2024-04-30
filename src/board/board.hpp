#pragma once 

#include <string> 
#include <ctype.h>
#include <iostream> 
#include <vector>
#include <memory> 
#include <utility>
#include <array> 

#include "piece.hpp" 
#include "enum.hpp"
#include "coordinate.hpp"
#include "helpers.hpp"

struct CastlingRights;

class Board {
public:
    std::array<std::shared_ptr<Piece>, 64> board;

    bool white_castle_short;
    bool white_castle_long;
    bool black_castle_short;
    bool black_castle_long;

    int en_passant_target; 
    int halfmove_clock; 
    int fullmove_clock; 

    int active_color;

    GameEndState game_end_state; 

    MoveType move_piece(int, int);
    void cast_ray(Piece*, int, int);
    void add_attack(Piece*, int);
    
    // Pseudo-legal moves to start. 
    void generate_legal_moves();

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

    Piece* piece_at(int); 

    static std::shared_ptr<Board> from_fen_string(std::string);

private: 
    Board();
    Board(std::array<std::shared_ptr<Piece>, 64>, int, CastlingRights, int, int, int);
    
    void increment_clock();
    int find_king_index_by_color(int);
    void prune_illegal_moves();
    void check_game_end_state();
    void calc_piece_vision(); 
    void check_en_pessant(int, int);
    void check_castling(int);  
    
    std::shared_ptr<Board> peek_next_position(int, int); 
}; 
