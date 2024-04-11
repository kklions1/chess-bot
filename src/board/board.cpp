#include "board.hpp" 

std::set<int> edges = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 23, 24,
     31, 32, 39, 40, 47, 48, 55, 56, 57, 58, 59, 60, 61, 62, 63 }; 

std::set<int> left_edges = { 0, 8, 16, 24, 32, 40, 48, 56 };
std::set<int> right_edges = { 7, 15, 23, 31, 39, 47, 55, 63 };
std::set<int> top_edges = { 0, 1, 2, 3, 4, 5, 6, 7 };
std::set<int> bottom_edges = { 56, 57, 58, 59, 60, 61, 62, 63 };

Board::Board() :
    white_castle_short(false), white_castle_long(false),
    black_castle_short(false), black_castle_long(false), 
    en_passant_target(-1), halfmove_clock(0), fullmove_clock(0),
    active_color(-1) {
        for (int i = 0; i < 64; ++i) { 
            board[i] = Piece::make_empty();
        }
    }

std::shared_ptr<Board> Board::copy() { 
    auto copied = std::make_shared<Board>();

    copied->white_castle_short = this->white_castle_short;
    copied->white_castle_long = this->white_castle_long;
    copied->black_castle_short = this->black_castle_short;
    copied->black_castle_long = this->black_castle_long;
    copied->en_passant_target = this->en_passant_target;
    copied->halfmove_clock = this->halfmove_clock;
    copied->fullmove_clock = this->fullmove_clock;
    copied->active_color = this->active_color;

    for (int i = 0; i < 64; ++i) { 
        copied->board[i] = this->board[i]->copy();
    }

    return copied;
}

MoveType Board::move_piece(int start, int target) { 
    std::cout << "moving piece " << start << " to " << target << std::endl;
    if (start == target) return MoveType::NO_MOVE; // The piece didnt move.

    // TODO pretty sure that whatever pointer manipulation i do here is a sin, but it works. Maybe fix?
    Piece_ptr move_target = this->board[start];
    Piece_ptr destination_target = this->board[target];

    if (move_target->color() != this->active_color) return MoveType::NO_MOVE;

    if (!move_target->vision.contains(target)) return MoveType::NO_MOVE; // invalid move

    if (destination_target->data == PieceType::EMPTY) { 
        Piece_ptr temp = destination_target;
        this->board[target] = move_target;
        this->board[start] = temp;

        increment_clock();
        generate_legal_moves();
        return MoveType::MOVE;
    }

    if (destination_target->color() != move_target->color()) { 
        this->board[target] = move_target; 
        this->board[start] = Piece::make_empty();

        increment_clock();
        generate_legal_moves();
        return MoveType::CAPTURE; 
    }

    return MoveType::NO_MOVE;
}

void Board::prune_illegal_moves(Piece_ptr piece, int piece_index) { 

}

std::shared_ptr<Board> Board::next_position(int start, int target) { 
    auto next_position = this->copy(); 
    switch (next_position->move_piece(start, target)) { 
        case MoveType::NO_MOVE: 
            return nullptr;
        case MoveType::CAPTURE: 
        case MoveType::MOVE: 
            return next_position; 
    }

}

void Board::generate_legal_moves() { 
    for (int i = 0; i < 64; ++i) { 
        Piece* piece = this->board[i].get();

        switch (piece->type()) { 
            case PieceType::ROOK: 
                rook_moves(piece, i);
                break;
            case PieceType::BISHOP: 
                bishop_moves(piece, i);
                break;
            case PieceType::PAWN:
                pawn_moves(piece, i);
                break;
            case PieceType::KNIGHT: 
                horsy_moves(piece, i);
                break;
            case PieceType::QUEEN:
                queen_moves(piece, i); 
                break;
            case PieceType::KING: 
                king_vision(piece, i);
                break;
            default: 
                break;
        }
    }
}

void Board::pawn_moves(Piece* piece, int index) { 
    std::set<int> white_starting_rank = {48, 49, 50, 51, 52, 53, 54, 55};
    std::set<int> black_starting_rank = {8, 9, 10, 11, 12, 13, 14, 15}; 
    piece->vision.clear();
    int left, right, advance, advance_twice; 
    bool on_starting_rank;

    if (piece->color() == PieceType::WHITE) { 
        left = index + Direction::NORTH_WEST;
        right = index + Direction::NORTH_EAST;
        advance = index + Direction::NORTH;
        advance_twice = advance + Direction::NORTH;
        on_starting_rank = white_starting_rank.contains(index);
    } else { // Black piece 
        right = index + Direction::SOUTH_EAST;
        left = index + Direction::SOUTH_WEST;
        advance = index + Direction::SOUTH;
        advance_twice = advance + Direction::SOUTH;
        on_starting_rank = black_starting_rank.contains(index);
    }

    if (this->board[advance]->data == PieceType::EMPTY) {
        piece->vision.insert(advance); 
    
        if (on_starting_rank && this->board[advance_twice]->data == PieceType::EMPTY)
            piece->vision.insert(advance_twice);
    } 
    
    Piece* left_target = this->board[left].get();
    Piece* right_target = this->board[right].get();
    
    // These if statements are why we need comments.
    if ( 
        ((left_target->data != PieceType::EMPTY
        && left_target->color() != piece->color()) 
        || (left_target->data == PieceType::EMPTY
            && left == this->en_passant_target))
        && !left_edges.contains(advance)
    ) piece->vision.insert(left);

    if (
        ((right_target->data != PieceType::EMPTY
        && right_target->color() != piece->color()) 
        || (right_target->data == PieceType::EMPTY
            && right == this->en_passant_target))
        && !right_edges.contains(advance)
    ) piece->vision.insert(right);
}

void Board::cast_ray(Piece* piece, int start, int direction) { 
    int current = start + direction;
    
    while (true) { 
        Piece* target = this->board[current].get();
        
        if (target->data == PieceType::EMPTY) { // Empty square
            piece->vision.insert(current);
            if (is_edge_in_direction(current, direction)) break; 
            current += direction;
            continue;
        }
        
        if (target->color() != piece->color()) { // Capture
            piece->vision.insert(current);
            break; // Last valid move in this direction
        }

        if (target->color() == piece->color()) { 
            break; // Friendly piece blocking our path. not a valid move.
        }
    }
}

void Board::add_attack(Piece* piece, int target_index) { 
    int color = piece->color();
    if (target_index < 0 || target_index > 63) { 
        return;
    }

    if (this->board[target_index]->color() != color) { 
        piece->vision.insert(target_index);
    }
}

void Board::rook_moves(Piece* piece, int index) { 
    piece->vision.clear();

    bool cast_west = !left_edges.contains(index);
    bool cast_east = !right_edges.contains(index);
    bool cast_north = !top_edges.contains(index);
    bool cast_south = !bottom_edges.contains(index);

    if (cast_west) {
        cast_ray(piece, index, Direction::WEST);
    }

    if (cast_east) {
        cast_ray(piece, index, Direction::EAST);
    }

    if (cast_south) {
        cast_ray(piece, index, Direction::SOUTH);
    }

    if (cast_north) {
        cast_ray(piece, index, Direction::NORTH);
    }
}

void Board::bishop_moves(Piece* piece, int index) { 
    piece->vision.clear();

    bool on_left_edge = left_edges.contains(index);
    bool on_right_edge = right_edges.contains(index);
    bool on_top_edge = top_edges.contains(index);
    bool on_bottom_edge = bottom_edges.contains(index);

    // cast ray up-left
    if (!on_left_edge && !on_top_edge) cast_ray(piece, index, Direction::NORTH_WEST);

    // cast ray up-right
    if (!on_right_edge && !on_top_edge) cast_ray(piece, index, Direction::NORTH_EAST);

    // cast ray down-left
    if (!on_left_edge && !on_bottom_edge) cast_ray(piece, index, Direction::SOUTH_WEST);

    // cast ray down-right
    if (!on_right_edge && !on_bottom_edge) cast_ray(piece, index, Direction::SOUTH_EAST);
}

void Board::horsy_moves(Piece* piece, int index) { 
    piece->vision.clear(); 

    std::set<int> inner_left = { 1, 9, 17, 25, 33, 41, 49, 57};
    std::set<int> inner_right = { 6, 14, 22, 30, 38, 46, 54, 62 };

    if (inner_left.contains(index)) { 
        // can't move two spaces west, can move one space west
        add_attack(piece, index + Direction::NNE);
        add_attack(piece, index + Direction::NNW);
        add_attack(piece, index + Direction::NEE);
        add_attack(piece, index + Direction::SSE);
        add_attack(piece, index + Direction::SSW);
        add_attack(piece, index + Direction::SEE);
        return;
    }

    if (inner_right.contains(index)) { 
        // can't move two spaces east, can move one space east
        add_attack(piece, index + Direction::NNE);
        add_attack(piece, index + Direction::NNW);
        add_attack(piece, index + Direction::NWW);
        add_attack(piece, index + Direction::SSE);
        add_attack(piece, index + Direction::SSW);
        add_attack(piece, index + Direction::SWW); 
        return;
    }

    if (left_edges.contains(index)) {
        // cant go west
        add_attack(piece, index + Direction::NNE);
        add_attack(piece, index + Direction::NEE);
        add_attack(piece, index + Direction::SSE);
        add_attack(piece, index + Direction::SEE);
        return;
    } 

    if (right_edges.contains(index)) {
        // cant go east
        add_attack(piece, index + Direction::NNW);
        add_attack(piece, index + Direction::NWW);
        add_attack(piece, index + Direction::SSW);
        add_attack(piece, index + Direction::SWW);
        return;
    } 
    
    if (top_edges.contains(index)) {
        // cant go north
        add_attack(piece, index + Direction::SSE);
        add_attack(piece, index + Direction::SEE);
        add_attack(piece, index + Direction::SSW);
        add_attack(piece, index + Direction::SWW);
        return;
    } 
    
    if (bottom_edges.contains(index)) {
        // cant go south    
        add_attack(piece, index + Direction::NNE);
        add_attack(piece, index + Direction::NEE);
        add_attack(piece, index + Direction::NNW);
        add_attack(piece, index + Direction::NWW);
        return; 
    } 

    if (!edges.contains(index)) {        
        add_attack(piece, index + Direction::NNE);
        add_attack(piece, index + Direction::NNW);
        add_attack(piece, index + Direction::NEE);
        add_attack(piece, index + Direction::NWW);
        add_attack(piece, index + Direction::SSE);
        add_attack(piece, index + Direction::SSW);
        add_attack(piece, index + Direction::SEE);
        add_attack(piece, index + Direction::SWW);
    }    
}

void Board::queen_moves(Piece* piece, int index) { 
    rook_moves(piece, index);
    auto temp_moves = piece->vision; // This has to exist because bishop_moves() clears the vision list 
    bishop_moves(piece, index);
    piece->vision.merge(temp_moves);
}   

void Board::king_vision(Piece* piece, int index) { 
    piece->vision.clear();

    bool on_left_edge = left_edges.contains(index);
    bool on_right_edge = right_edges.contains(index);
    bool on_top_edge = top_edges.contains(index);
    bool on_bottom_edge = bottom_edges.contains(index);

    if (!on_left_edge) { 
        int w_target = index + Direction::WEST; 
        if (this->board[w_target]->data == PieceType::EMPTY || this->board[w_target]->color() != piece->color()) { 
            piece->vision.insert(w_target);
        }
    } 

    if (!on_right_edge) { 
        int e_target = index + Direction::EAST; 
        if (this->board[e_target]->data == PieceType::EMPTY || this->board[e_target]->color() != piece->color()) { 
            piece->vision.insert(e_target);
        }
    }

    if (!on_bottom_edge) { 
        int s_target = index + Direction::SOUTH; 
        if (this->board[s_target]->data == PieceType::EMPTY || this->board[s_target]->color() != piece->color()) { 
            piece->vision.insert(s_target);
        }
    }

    if (!on_top_edge) { 
        int n_target = index + Direction::NORTH; 
        if (this->board[n_target]->data == PieceType::EMPTY || this->board[n_target]->color() != piece->color()) { 
            piece->vision.insert(n_target);
        }
    }
    
    if (!on_left_edge && !on_top_edge) { 
        int nw_target = index + Direction::NORTH_WEST; 
        if (this->board[nw_target]->data == PieceType::EMPTY || this->board[nw_target]->color() != piece->color()) { 
            piece->vision.insert(nw_target);
        }
    }

    if (!on_left_edge && !on_bottom_edge) { 
        int sw_target = index + Direction::SOUTH_WEST; 
        if (this->board[sw_target]->data == PieceType::EMPTY || this->board[sw_target]->color() != piece->color()) { 
            piece->vision.insert(sw_target);
        }
    }

    if (!on_right_edge && !on_top_edge) { 
        int ne_target = index + Direction::NORTH_EAST; 
        if (this->board[ne_target]->data == PieceType::EMPTY || this->board[ne_target]->color() != piece->color()) { 
            piece->vision.insert(ne_target);
        }
    }

    if (!on_right_edge && !on_bottom_edge) { 
        int sw_target = index + Direction::SOUTH_WEST; 
        if (this->board[sw_target]->data == PieceType::EMPTY || this->board[sw_target]->color() != piece->color()) { 
            piece->vision.insert(sw_target);
        }
    }

    int color = piece->color(); 

    switch (color) { 
        case PieceType::WHITE: { 
            if (this->white_castle_long) { 
                piece->vision.insert(index + (2 * Direction::WEST));
            }
            
            if (this->white_castle_short) { 
                piece->vision.insert(index + (2 * Direction::EAST));
            }

            break;
        }
        case PieceType::BLACK: { 
            if (this->black_castle_long) { 
                piece->vision.insert(index + (2 * Direction::WEST));
            }   
            
            if (this->black_castle_short) { 
                piece->vision.insert(index + (2 * Direction::EAST));
            }

            break;
        }
        default: 
            // TODO handle error. We shouldn't ever get here tho so im just gonna ignore it for a while until my own incompetence catches up to me c:
            break;
    }

}

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
        Piece* current_piece = board.board[index].get(); 
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
}
    
void parse_active_color(const std::string& fen, Board& board) { 
    char active_color = fen.c_str()[0];
    if (active_color == 'w') {
        board.active_color = PieceType::WHITE;
        return;
    } 

    if (active_color == 'b') { 
        board.active_color = PieceType::BLACK;
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
        if (board.board[0]->data == PieceType::EMPTY) { 
            skip_count++;
            continue;
        }

        if (skip_count != 0) { 
            // fen.append();
            skip_count = 0;
        }
        
        switch (board.board[i]->data) { 
            case PieceType::BLACK | PieceType::PAWN: 
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

bool in_check(int color, const Board& board) { 

    return false;
}

void Board::increment_clock() { 
    this->halfmove_clock++; 
    if (this->active_color == PieceType::WHITE) { 
        this->active_color = PieceType::BLACK;
        return;
    } 

    if (this->active_color == PieceType::BLACK) { 
        this->active_color = PieceType::WHITE;
        this->fullmove_clock++;
        return;
    }
}