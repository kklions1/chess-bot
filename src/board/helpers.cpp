#include "helpers.hpp" 

std::set<int> edges = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 23, 24,
     31, 32, 39, 40, 47, 48, 55, 56, 57, 58, 59, 60, 61, 62, 63 }; 

std::set<int> left_edges = { 0, 8, 16, 24, 32, 40, 48, 56 };
std::set<int> right_edges = { 7, 15, 23, 31, 39, 47, 55, 63 };
std::set<int> top_edges = { 0, 1, 2, 3, 4, 5, 6, 7 };
std::set<int> bottom_edges = { 56, 57, 58, 59, 60, 61, 62, 63 };

void print_board(const Board& board) { 
    int counter = 0;
    std::string result = ""; 

    for (int i = 0; i < 64; ++i) { 
        switch (board.board[i]->data) { 
            case PieceType::BLACK | PieceType::PAWN: 
                result.append("p");
                break; 
            case PieceType::WHITE | PieceType::PAWN: 
                result.append("P");
                break;
            case PieceType::BLACK | PieceType::KNIGHT: 
                result.append("n");
                break;
            case PieceType::WHITE | PieceType::KNIGHT: 
                result.append("N");
                break; 
            case PieceType::BLACK | PieceType::BISHOP: 
                result.append("b");
                break;
            case PieceType::WHITE | PieceType::BISHOP: 
                result.append("B");
                break;
            case PieceType::BLACK | PieceType::ROOK: 
                result.append("r");
                break;
            case PieceType::WHITE | PieceType::ROOK: 
                result.append("R");
                break;
            case PieceType::BLACK | PieceType::QUEEN:
                result.append("q");
                break;
            case PieceType::WHITE | PieceType::QUEEN: 
                result.append("Q");
                break;
            case PieceType::BLACK | PieceType::KING:
                result.append("k");
                break;
            case PieceType::WHITE | PieceType::KING:
                result.append("K");
                break;
            case PieceType::EMPTY: 
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

int parse_active_color(const std::string& fen) { 
    char active_color = fen.c_str()[0];
    if (active_color == 'w') {
        return PieceType::WHITE;
    } 

    if (active_color == 'b') { 
        return PieceType::BLACK;
    }

    throw std::runtime_error("unable to parse active color");
}

std::array<std::shared_ptr<Piece>, 64> parse_piece_locations(const std::string& fen) { 
    auto it = fen.begin(); 
    int index = 0;
   
    std::array<std::shared_ptr<Piece>, 64> result; 
    for (int i = 0; i < 64; ++i) { 
        result[i] = Piece::make_empty();
    }

    while (it != fen.end()) { 
        Piece* current_piece = result[index].get();
        if (isdigit(*it)) { 
            index += (*it - '0'); 
        } else { 
            switch (*it) { 
                case 'p':
                    current_piece->data = PieceType::BLACK | PieceType::PAWN; 
                    index++;
                    break; 
                case 'P': 
                    current_piece->data = PieceType::WHITE | PieceType::PAWN;
                    index++;
                    break;
                case 'n': 
                    current_piece->data = PieceType::BLACK | PieceType::KNIGHT;
                    index++;
                    break;
                case 'N': 
                    current_piece->data = PieceType::WHITE | PieceType::KNIGHT; 
                    index++;
                    break; 
                case 'b': 
                    current_piece->data = PieceType::BLACK | PieceType::BISHOP;
                    index++;
                    break;
                case 'B': 
                    current_piece->data = PieceType::WHITE | PieceType::BISHOP;
                    index++;
                    break;
                case 'r': 
                    current_piece->data = PieceType::BLACK | PieceType::ROOK;
                    index++;
                    break;
                case 'R': 
                    current_piece->data = PieceType::WHITE | PieceType::ROOK;
                    index++;
                    break;
                case 'q':
                    current_piece->data = PieceType::BLACK | PieceType::QUEEN;
                    index++;
                    break;
                case 'Q': 
                    current_piece->data = PieceType::WHITE | PieceType::QUEEN;
                    index++;
                    break;
                case 'k':
                    current_piece->data = PieceType::BLACK | PieceType::KING;
                    index++;
                    break;
                case 'K':
                    current_piece->data = PieceType::WHITE | PieceType::KING;
                    index++;
                    break;
                case '/': 
                    // skip, should go to next rank
                    break;
            }
        }
        it++;
    }

    return result;
}

CastlingRights parse_castling_rights(const std::string& fen) { 
    auto it = fen.begin();

    bool white_castle_long = false;
    bool black_castle_long = false; 
    bool white_castle_short = false;
    bool black_castle_short = false; 

    while (it != fen.end()) { 
        switch (*it) { 
            case 'Q': 
                white_castle_long = true;
                break;
            case 'q': 
                black_castle_long = true; 
                break;
            case 'K': 
                white_castle_short = true; 
                break;
            case 'k': 
                black_castle_short = true; 
                break;
            case '-': 
                // no one has castling rights, which is the default in my board
                break;
        }

        it++;
    }

    return CastlingRights(white_castle_long, white_castle_long, black_castle_long, black_castle_short);
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

void print_index() {
  int counter = 0; 
  for (int i = 0; i < 64; ++i) {
    if (counter == 8) {
      std::cout << std::endl;
      counter = 0;
    }
    std::cout << i << " ";
    counter++;
  }
  std::cout << std::endl;
}

bool is_edge_index(int i) { 
    return left_edges.contains(i) || right_edges.contains(i) || top_edges.contains(i) || bottom_edges.contains(i);
}

bool is_edge_in_direction(int i, int direction) { 
    switch (direction) { 
        case Direction::NORTH: 
            return top_edges.contains(i);
        case Direction::SOUTH: 
            return bottom_edges.contains(i);
        case Direction::EAST: 
            return right_edges.contains(i);
        case Direction::WEST: 
            return left_edges.contains(i);
        case Direction::NORTH_EAST: 
            return top_edges.contains(i) || right_edges.contains(i);
        case Direction::NORTH_WEST: 
            return top_edges.contains(i) || left_edges.contains(i);
        case Direction::SOUTH_EAST: 
            return bottom_edges.contains(i) || right_edges.contains(i);
        case Direction::SOUTH_WEST: 
            return bottom_edges.contains(i) || left_edges.contains(i);
        default:
            return is_edge_index(i);
    }
}
