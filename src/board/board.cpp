#include "board.hpp" 

void print_board(const Board& board) { 
    int counter = 0;
    std::string result = ""; 

    for (int i = 0; i < 64; ++i) { 
        switch (board.board[i]) { 
            case Piece::BLACK | Piece::PAWN: 
                result.append("p");
                break; 
            case Piece::WHITE | Piece::PAWN: 
                result.append("P");
                break;
            case Piece::BLACK | Piece::KNIGHT: 
                result.append("n");
                break;
            case Piece::WHITE | Piece::KNIGHT: 
                result.append("N");
                break; 
            case Piece::BLACK | Piece::BISHOP: 
                result.append("b");
                break;
            case Piece::WHITE | Piece::BISHOP: 
                result.append("B");
                break;
            case Piece::BLACK | Piece::ROOK: 
                result.append("r");
                break;
            case Piece::WHITE | Piece::ROOK: 
                result.append("R");
                break;
            case Piece::BLACK | Piece::QUEEN:
                result.append("q");
                break;
            case Piece::WHITE | Piece::QUEEN: 
                result.append("Q");
                break;
            case Piece::BLACK | Piece::KING:
                result.append("k");
                break;
            case Piece::WHITE | Piece::KING:
                result.append("K");
                break;
            case Piece::EMPTY: 
                result.append("0");
                break;
        }

        result.append(" ");
        counter++; 
        if (counter == 8) { 
            result.append("\n"); 
            counter = 0;
        }
    }

    std::cout << result << std::endl;
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
                    index++;
                    break; 
                case 'P': 
                    *square = Piece::WHITE | Piece::PAWN; 
                    index++;
                    break;
                case 'n': 
                    *square = Piece::BLACK | Piece::KNIGHT;
                    index++;
                    break;
                case 'N': 
                    *square = Piece::WHITE | Piece::KNIGHT; 
                    index++;
                    break; 
                case 'b': 
                    *square = Piece::BLACK | Piece::BISHOP;
                    index++;
                    break;
                case 'B': 
                    *square = Piece::WHITE | Piece::BISHOP;
                    index++;
                    break;
                case 'r': 
                    *square = Piece::BLACK | Piece::ROOK;
                    index++;
                    break;
                case 'R': 
                    *square = Piece::WHITE | Piece::ROOK;
                    index++;
                    break;
                case 'q':
                    *square = Piece::BLACK | Piece::QUEEN;
                    index++;
                    break;
                case 'Q': 
                    *square = Piece::WHITE | Piece::QUEEN;
                    index++;
                    break;
                case 'k':
                    *square = Piece::BLACK | Piece::KING;
                    index++;
                    break;
                case 'K':
                    *square = Piece::WHITE | Piece::KING;
                    index++;
                    break;
                case '/': 
                    // skip, should go to next rank
                    break;
            }
        }
        it++;
    }
}
    
void parse_active_color(const std::string& fen, Board& board) { 
    char active_color = fen.c_str()[0];
    if (active_color == 'w') {
        board.active_color = Piece::WHITE;
        return;
    } 

    if (active_color == 'b') { 
        board.active_color = Piece::BLACK;
        return;
    }
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
    std::string fen; 
    // The counter for where to place '/' in the fen string. when this is 8, we need a new line 
    int rank_count = 0;
    
    // The counter for when we encounter empty spaces. Every empty space adds 1 to the counter, when a non-empty square is reached
    // I need to append this count to the fen string only if it is greater than 0. If we do append it, it should be reset back to 0
    int skip_count = 0;

    for (int i = 0; i < 64; i++) { 
        if (board.board[0] == Piece::EMPTY) { 
            skip_count++;
            continue;
        }

        if (skip_count != 0) { 
            // fen.append();
            skip_count = 0;
        }
        
        switch (board.board[i]) { 
            case Piece::BLACK | Piece::PAWN: 
                break;
                
        }

        // switch (board.board[i]) { 
        //     case Piece::BLACK | Piece::PAWN: 
              
        // }
    }

    return fen;
}

// accepts a position string in algebraic notation, ex: e4, g6, h5
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

bool is_color(int piece, int color_mask) { 
    return (piece & color_mask) == color_mask;
}

int piece_type(int piece) {
    return piece & 0b00111;
}

void print_index() {
  int counter = 0; 
  for (int i = 0; i < 64; ++i) {
    if (counter == 8) {
      std::cout << std::endl;
      counter = 0;
    }
    std::cout << i << " ";
  }
}
