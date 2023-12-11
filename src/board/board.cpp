#include "board.hpp" 
#include "../common/common_structs.hpp"

#include <string> 
#include <ctype.h>
#include <iostream> 

void print_board(const Board& board) { 
    
    for (int i = 0; i < 64; ++i) { 
        // TODO implement something 
    }
}

void parse_fen_string(const std::string& fen, Board& board) { 
    int index = 0; 
    auto current = fen.begin(); 

    // parse the positions of the pieces on the board
    while (*current != ' ') { 
        int* current_square = &board.board[index];

        if (isdigit(*current)) { 
            index += (*current - '0'); 
        } else { 
            switch (*current) { 
                case 'p':
                    *current_square = Piece::BLACK | Piece::PAWN; 
                    break; 
                case 'P': 
                    *current_square = Piece::WHITE | Piece::PAWN; 
                    break;
                case 'n': 
                    *current_square = Piece::BLACK | Piece::KNIGHT;
                    break;
                case 'N': 
                    *current_square = Piece::WHITE | Piece::KNIGHT; 
                    break; 
                case 'b': 
                    *current_square = Piece::BLACK | Piece::BISHOP;
                    break;
                case 'B': 
                    *current_square = Piece::WHITE | Piece::BISHOP;
                    break;
                case 'r': 
                    *current_square = Piece::BLACK | Piece::ROOK;
                    break;
                case 'R': 
                    *current_square = Piece::WHITE | Piece::ROOK;
                    break;
                case 'q':
                    *current_square = Piece::BLACK | Piece::QUEEN;
                    break;
                case 'Q': 
                    *current_square = Piece::WHITE | Piece::QUEEN;
                    break;
                case 'k':
                    *current_square = Piece::BLACK | Piece::KING;
                    break;
                case 'K':
                    *current_square = Piece::WHITE | Piece::KING;
                    break;
                case '/': 
                    // skip, should go to next rank
                    break;
            }
            
            index++;
        }
    }

    // parse the rest of the fen string 
    current++;
    board.active_color = *current; 

    current++;
    while (*current != ' ') { 
        switch (*current) { 
            case 'Q': 
                board.white_castle_long = true;
                break;
            case 'q': 
                board.black_castle_long = true; 
                break;
            case 'K': 
                board.white_castle_short = true; 
                break;
            case 'k': 
                board.black_castle_short = true; 
                break;
            case '-': 
                // no one has castling rights, which is the default in my board
                break;
        }
    }
}

std::string generate_fen_string(const Board& board) { 
    return "";
}


int get_board_index(const std::string& position) { 
    return -1;
}