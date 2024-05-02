#include "board.hpp" 

Board::Board() :
    white_castle_short(false), white_castle_long(false),
    black_castle_short(false), black_castle_long(false), 
    en_passant_target(-1), halfmove_clock(0), fullmove_clock(0),
    game_end_state(GameEndState::PLAYING),
    active_color(-1) {
        for (int i = 0; i < 64; ++i) { 
            board[i] = Piece::make_empty();
        }
    }

Board::Board(
    std::array<std::shared_ptr<Piece>, 64> pieces, 
    int active_color, 
    CastlingRights castling_rights,
    int en_passant_target, 
    int halfmove_clock,
    int fullmove_clock
) : 
    board(pieces),
    active_color(active_color),
    white_castle_long(castling_rights.white_castle_long),
    white_castle_short(castling_rights.white_castle_short), 
    black_castle_long(castling_rights.black_castle_long),
    black_castle_short(castling_rights.black_castle_short),
    en_passant_target(en_passant_target),
    halfmove_clock(halfmove_clock), 
    fullmove_clock(fullmove_clock),
    game_end_state(GameEndState::PLAYING)
    { }

std::shared_ptr<Board> Board::from_fen_string(std::string fen) { 
    std::vector<std::string> fen_split = split(fen, ' ');

    if (fen_split.size() != 6) { 
        // TODO how do C++ exceptions work exactly? what happens if this is thrown and i dont have a try/catch?
        throw std::runtime_error("fen string appears to be invalid");
    }

    std::array<std::shared_ptr<Piece>, 64> pieces = parse_piece_locations(fen_split[0]);
    
    int active_color = parse_active_color(fen_split[1]);

    CastlingRights castling_rights = parse_castling_rights(fen_split[2]);

    int en_passant_target = -1; 
    if (fen_split[3].c_str()[0] != '-') { 
        en_passant_target = get_board_index(fen_split[3].c_str());
    }

    int halfmove_clock = std::stoi(fen_split[4]); 
    int fullmove_clock = std::stoi(fen_split[5]); 

    return std::shared_ptr<Board>(
        new Board(pieces,
            active_color,
            castling_rights,  
            en_passant_target, 
            halfmove_clock, 
            fullmove_clock
            )
        );
    
}

Piece* Board::piece_at(int index) {
    if (index < 0 || index >= 64) { return nullptr; }
    
    return this->board[index].get(); 
}    

std::shared_ptr<Board> Board::clone() { 
    std::array<std::shared_ptr<Piece>, 64> pieces_copy; 
    for (int i = 0; i < 64; ++i) { 
        pieces_copy[i] = Piece::make_empty();
        pieces_copy[i]->data = this->board[i]->data;
        pieces_copy[i]->vision.insert(this->board[i]->vision.begin(), this->board[i]->vision.end());
    }

    return std::shared_ptr<Board>(new Board(
        pieces_copy,
        active_color,
        CastlingRights(
            white_castle_long,
            white_castle_short,
            black_castle_long,
            black_castle_short
        ),  
        en_passant_target, 
        halfmove_clock, 
        fullmove_clock
    )); 

}

// take a peek at what pieces will be able to see if a move is executed
std::shared_ptr<Board> Board::peek_next_position(int start, int target) { 
    if (start == target) return nullptr; // The piece didnt move.
    
    auto next = this->clone();

    // TODO use move() function
    std::shared_ptr<Piece> move_target = next->board[start];
    std::shared_ptr<Piece> destination_target = next->board[target];

    if (move_target->color() != next->active_color) return nullptr;

    if (!move_target->vision.contains(target)) return nullptr; // invalid move

    if (destination_target->data == PieceType::EMPTY) { 
        std::shared_ptr<Piece> temp = destination_target;
        next->board[target] = move_target;
        next->board[start] = temp;

        next->en_passant_target = -1; 
        next->check_en_pessant(start, target);
        next->check_castling(target);
        next->increment_clock();
        next->calc_piece_vision();
        return next;
    }

    if (destination_target->color() != move_target->color()) { 
        next->board[target] = move_target; 
        next->board[start] = Piece::make_empty();

        next->en_passant_target = -1; 
        next->check_en_pessant(start, target);
        next->check_castling(target);
        next->increment_clock();
        next->calc_piece_vision();
        return next; 
    }

    return nullptr;
}

MoveType Board::move_piece(int start, int target) { 
   
     // std::cout << "moving piece " << start << " to " << target << std::endl;
    if (start == target) return MoveType::NO_MOVE; // The piece didnt move.
    
    // TODO pretty sure that whatever pointer manipulation i do here is a sin, but it works. Maybe fix?
    Piece* move_piece = piece_at(start);
    Piece* target_piece = piece_at(target);

    if (move_piece->color() != this->active_color) return MoveType::NO_MOVE;

    if (!move_piece->vision.contains(target)) return MoveType::NO_MOVE; // invalid move

    if (target_piece->data == PieceType::EMPTY) { 
        move(move_piece, target_piece);
        this->en_passant_target = -1;
        check_en_pessant(start, target);
        check_castling(target);
        increment_clock();
        generate_legal_moves();
        return MoveType::MOVE;
    }

    if (target_piece->color() != move_piece->color()) { 
        move(move_piece, target_piece);

        this->en_passant_target = -1;
        check_en_pessant(start, target);
        check_castling(target);
        increment_clock();
        generate_legal_moves();
        return MoveType::CAPTURE; 
    }

    return MoveType::NO_MOVE;
}

void Board::prune_illegal_moves() { 
    for (int i = 0; i < 64; ++i) { 
        Piece* current = this->board[i].get();

        if (current->data == PieceType::EMPTY) { continue; }

        std::set<int> invalid_moves; 

        for (int target : current->vision) { 
            auto peek = peek_next_position(i, target);

            if (peek == nullptr) { continue; }

            if (peek->nap_in_check()) { 
                invalid_moves.insert(target);
            }
        }

        if (invalid_moves.empty()) { continue; }

        for (auto it = invalid_moves.begin(); it != invalid_moves.end(); it++) { 
            current->vision.erase(*it);
        }

        invalid_moves.clear();
    }
}

std::shared_ptr<Board> Board::next_position(int start, int target) { 
    auto next_position = this->clone(); 
    switch (next_position->move_piece(start, target)) { 
        case MoveType::NO_MOVE: 
            return nullptr;
        case MoveType::CAPTURE: 
        case MoveType::MOVE: 
            return next_position; 
    }
}

void Board::calc_piece_vision() { 
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

void Board::generate_legal_moves() { 
    calc_piece_vision();
    prune_illegal_moves();
    check_game_end_state(); 
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

    if (index_inbounds(advance)) { 
        if (this->board[advance]->data == PieceType::EMPTY) {
            piece->vision.insert(advance); 
    
            if (on_starting_rank && this->board[advance_twice]->data == PieceType::EMPTY)
                piece->vision.insert(advance_twice);
        }
    }
     
    
    Piece* left_target = piece_at(left);
    Piece* right_target = piece_at(right);

    // These if statements are why we need comments.
    if (left_target != nullptr) { 
        if ( 
            left_target != nullptr &&
            ((left_target->data != PieceType::EMPTY
            && left_target->color() != piece->color()) 
            || (left_target->data == PieceType::EMPTY
                && left == this->en_passant_target))
            && !left_edges.contains(advance)
        ) piece->vision.insert(left);
    }
    
    if (right_target != nullptr) { 
        if (
            right_target != nullptr &&
            ((right_target->data != PieceType::EMPTY
            && right_target->color() != piece->color()) 
            || (right_target->data == PieceType::EMPTY
                && right == this->en_passant_target))
            && !right_edges.contains(advance)
        ) piece->vision.insert(right);    
    }
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
        if (this->board[w_target]->data == PieceType::EMPTY 
            || this->board[w_target]->color() != piece->color()) { 
            piece->vision.insert(w_target);
        }
    } 

    if (!on_right_edge) { 
        int e_target = index + Direction::EAST; 
        if (this->board[e_target]->data == PieceType::EMPTY 
            || this->board[e_target]->color() != piece->color()) { 
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
        int se_target = index + Direction::SOUTH_EAST; 
        if (this->board[se_target]->data == PieceType::EMPTY || this->board[se_target]->color() != piece->color()) { 
            piece->vision.insert(se_target);
        }
    }

    int color = piece->color(); 

    switch (color) { 
        case PieceType::WHITE: { 
            if (this->white_castle_long) {
                int castle_target = index + (2 * Direction::WEST); 
                if (this->board[castle_target]->data == PieceType::EMPTY) { piece->vision.insert(castle_target); } 
            }
            
            if (this->white_castle_short) { 
                int castle_target = index + (2 * Direction::EAST); 
                if (this->board[castle_target]->data == PieceType::EMPTY) { piece->vision.insert(castle_target); } 
            }

            break;
        }
        case PieceType::BLACK: { 
            if (this->black_castle_long) { 
                int castle_target = index + (2 * Direction::WEST); 
                if (this->board[castle_target]->data == PieceType::EMPTY) { piece->vision.insert(castle_target); } 
            }   
            
            if (this->black_castle_short) { 
                int castle_target = index + (2 * Direction::EAST); 
                if (this->board[castle_target]->data == PieceType::EMPTY) { piece->vision.insert(castle_target); } 
            }

            break;
        }
        default: 
            // TODO handle error. We shouldn't ever get here tho so im just gonna ignore it for a while until my own incompetence catches up to me c:
            break;
    }

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

int Board::find_king_index_by_color(int color) { 
    for (int i = 0; i < 64; ++i) { 
        if (this->board[i]->data == (PieceType::KING | color)) { 
            return i; 
        }
    }

    throw std::runtime_error("King not found"); 
}

int Board::get_ap_king_index() { 
    int active_color = this->active_color; 

    for (int i = 0; i < 64; ++i) { 
        if (this->board[i]->data == (PieceType::KING | active_color)) { 
            return i; 
        }
    }

    throw std::runtime_error("King not found"); 
}

bool Board::active_player_in_check() { 
    int active_color = this->active_color; 
    int ap_king_index = get_ap_king_index();

    for (int i = 0; i < 64; ++i) { 
        Piece* current = this->board[i].get(); 

        if (current->data == PieceType::EMPTY) { continue; }

        if (current->color() == active_color) { 
            continue;
        }        

        for (int target : current->vision) { 
            if (target == ap_king_index) { 
                return true;
            }
        }
    }

    return false;
}

// non-active player, or "The person who just played a move"
bool Board::nap_in_check() { 
    int inactive_color;     
    if (this->active_color == PieceType::WHITE) { 
        inactive_color = PieceType::BLACK; 
    } else { 
        inactive_color = PieceType::WHITE; 
    }

    int nap_king_index = find_king_index_by_color(inactive_color);

    for (int i = 0; i < 64; ++i) { 
        Piece* current = this->board[i].get(); 

        if (current->data == PieceType::EMPTY) { continue; }

        if (current->color() == inactive_color) { continue; }  

        for (int target : current->vision) { 
            if (target == nap_king_index) { 
                return true;
            }
        }
    }

    return false;
}

void Board::check_en_pessant(int start, int target) { 
    if (this->board[start]->type() != PieceType::PAWN) { return; }

    int distance = target - start; 
    if (distance == (Direction::NORTH * 2)) { 
        this->en_passant_target = target + Direction::SOUTH; 
    }

    if (distance == (Direction::SOUTH * 2)) { 
        this->en_passant_target = target + Direction::NORTH; 
    }
}

void Board::check_castling(int moved_piece_index) { 
    Piece* moved = this->board[moved_piece_index].get();
    int moved_type = moved->type(); 
    
    if (moved_type != PieceType::KING && moved_type != PieceType::ROOK) { return; }

    // simple, if the king moves, you can castle anymore. even if the move was a castle.
    if (moved_type == PieceType::KING) { 
        if (moved->color() == PieceType::WHITE) { 
            this->white_castle_long = false;
            this->white_castle_short = false;
        } else { 
            this->black_castle_long = false;
            this->black_castle_short = false;
        }

        return;
    }

    // If we get here, the piece must be a rook. no sense in checking it again.
    if (moved->color() == PieceType::WHITE) { 
        
    } else { 

    }

}

void Board::check_game_end_state() { 
    bool has_legal_moves = false; 
    for (int i = 0; i < 64; ++i) { 
        Piece* current = piece_at(i); 
        if (current->color() != active_color) { continue; } 
        if (!current->vision.empty()) { 
            has_legal_moves = true; 
            break;
        }
    }

    if (has_legal_moves) { 
        this->game_end_state = GameEndState::PLAYING;
        return;
    }

    // If there are no legal moves....
    if (active_player_in_check()) { 
        this->game_end_state = GameEndState::CHECKMATE; 
    } else { 
        this->game_end_state = GameEndState::STALEMATE; 
    }
}

