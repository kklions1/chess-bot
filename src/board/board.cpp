#include "board.hpp" 

void print_board(const Board& board) { 
    for (int i = 0; i < 64; ++i) { 
        // TODO implement something 
    }
}

std::vector<std::string> split(std::string s, const char delim) { 
    std::vector<std::string> result; 
    std::size_t pos;
    std::string token; 

    while ((pos = s.find(delim)) != std::string::npos) { 
        token = s.substr(0, pos); 
        result.push_back(token);
        s.erase(0, pos + 1); 
    }

    result.push_back(s);

    return result; 
}

void parse_piece_locations(const std::string& fen, Board& board) { 
    auto it = fen.begin(); 
    int index = 0;

    while (it != fen.end()) { 
        int* square = &board.board[index]; 
        if (isdigit(*it)) { 
            index += (*it - '0'); 
        } else { 
            switch (*it) { 
                case 'p':
                    *square = Piece::BLACK | Piece::PAWN; 
                    break; 
                case 'P': 
                    *square = Piece::WHITE | Piece::PAWN; 
                    break;
                case 'n': 
                    *square = Piece::BLACK | Piece::KNIGHT;
                    break;
                case 'N': 
                    *square = Piece::WHITE | Piece::KNIGHT; 
                    break; 
                case 'b': 
                    *square = Piece::BLACK | Piece::BISHOP;
                    break;
                case 'B': 
                    *square = Piece::WHITE | Piece::BISHOP;
                    break;
                case 'r': 
                    *square = Piece::BLACK | Piece::ROOK;
                    break;
                case 'R': 
                    *square = Piece::WHITE | Piece::ROOK;
                    break;
                case 'q':
                    *square = Piece::BLACK | Piece::QUEEN;
                    break;
                case 'Q': 
                    *square = Piece::WHITE | Piece::QUEEN;
                    break;
                case 'k':
                    *square = Piece::BLACK | Piece::KING;
                    break;
                case 'K':
                    *square = Piece::WHITE | Piece::KING;
                    break;
                case '/': 
                    // skip, should go to next rank
                    break;
            }
            index++;
        }
        it++;
    }
}
    
void parse_active_color(const std::string& fen, Board& board) { 
    board.active_color = fen.c_str()[0];
}

void parse_castling_rights(const std::string& fen, Board& board) { 
    auto it = fen.begin();

    while (it != fen.end()) { 
        switch (*it) { 
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

        it++;
    }
}

void parse_en_passant_targets(const std::string& fen, Board& board) { 
    // Parse en-passant targets 
    const char* target = fen.c_str(); 

    if (target[0] == '-') { 
        board.en_passant_target = -1;
    } else { 
        board.en_passant_target = get_board_index(target);
    }
}

void parse_halfmove_clock(const std::string& fen, Board& board) { 
    board.halfmove_clock = std::stoi(fen);
}

void parse_fullmove_clock(const std::string& fen, Board& board) { 
    board.fullmove_clock = std::stoi(fen);
}

void parse_fen_string(std::string fen, Board& board) { 
    std::vector<std::string> fen_split = split(fen, ' ');

    if (fen_split.size() != 6) { 
        // TODO how do C++ exceptions work exactly? what happens if this is thrown and i dont have a try/catch?
        throw std::runtime_error("fen string appears to be invalid");
    }

    parse_piece_locations(fen_split[0], board);
    
    parse_active_color(fen_split[1], board);

    parse_castling_rights(fen_split[2], board);

    parse_en_passant_targets(fen_split[3], board);

    parse_halfmove_clock(fen_split[4], board);

    parse_fullmove_clock(fen_split[5], board);

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