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
        current++;
    }

    // parse the rest of the fen string 
    current++; // skip the space
    
    board.active_color = *current; 
    current++; // next position

    current++; // skip the space 

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
        current++;
    }

    current++; // skips the space 

    // Parse en-passant targets 
    if (*current == '-') { 
        board.en_passant_target = -1;
        current++; 
    } else { 
        char space[2]; 
        space[0] = *current;
        current++;
        space[1] = *current;
        board.en_passant_target = get_board_index(space);
        current++;
    }

    current++; // skips whitespace 

    // parse halfmove clock
    char* buffer = new char[3]; 
    int i = 0;
    while (*current != ' ') { 
        buffer[i] = *current; 
        current++;
        i++;
    }

    board.halfmove_clock = std::stoi(buffer); 

    current++; 

    // parse fullmove clock 
    i = 0; 
    while (*current != ' ' || *current == '\0') { 
        buffer[i] = *current; 
        current++;
        i++;
    }

    board.fullmove_clock = std::stoi(buffer); 

    delete [] buffer;
}

std::string generate_fen_string(const Board& board) { 
    return "";
}

// accepts a position string in algebraic notation, ex: e4
int get_board_index(const char* square) { 
    int rank_offset = 0;
    int file_offset = 0; 

    switch (square[0]) { 
        case 'a': 
            file_offset = 0;
            break;
        case 'b': 
            file_offset = 1; 
            break;
        case 'c':
            file_offset = 2;
            break;
        case 'd':
            file_offset = 3;
            break;
        case 'e': 
            file_offset = 4;
            break;
        case 'f': 
            file_offset = 5;
            break;
        case 'g': 
            file_offset = 6;
            break;
        case 'h': 
            file_offset = 7;
            break;
    }
    
    switch(square[1]) { 
        case '1': 
            rank_offset = 7;
            break;
        case '2':
            rank_offset = 6;
            break;
        case '3': 
            rank_offset = 5;
            break;
        case '4': 
            rank_offset = 4;
            break;
        case '5': 
            rank_offset = 3;
            break;
        case '6': 
            rank_offset = 2;
            break;
        case '7': 
            rank_offset = 1;
            break;
        case '8': 
            rank_offset = 0;
            break;
    }

    return (rank_offset * 8) + file_offset;
}