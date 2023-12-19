#include "engine.hpp"

#include <iostream> 

std::vector<Move> generate_legal_moves(const Board& board) { 
    std::vector<Move> legal_moves; 
    for (int i = 0; i < 64; i++) { 
        int piece = board.board[i]; 
        if (is_color(piece, board.active_color)) { 
            switch (piece_type(piece)) { 
                case Piece::PAWN:  
                    // generate_pawn_moves(i);
                    break;
                case Piece::KNIGHT: 
                    break;
                case Piece::BISHOP: 
                    break;
                case Piece::ROOK: 
                    break;
                case Piece::QUEEN: 
                    break;
                case Piece::KING: 
                    break;
            }
        }
    }

    return legal_moves;
}

// void generate_pawn_moves(int index) { 

// }