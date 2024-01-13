#include "piece.hpp"

#include <iostream>

int color_mask = 0b11000;
int piece_mask = 0b00111;

std::set<int> edges = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 23, 24,
     31, 32, 39, 40, 47, 48, 55, 56, 57, 58, 59, 60, 61, 62, 63 }; 

std::set<int> left_edges = { 0, 8, 16, 24, 32, 40, 48, 56 };
std::set<int> right_edges = { 7, 15, 23, 31, 39, 47, 55, 63 };
std::set<int> top_edges = { 0, 1, 2, 3, 4, 5, 6, 7 };
std::set<int> bottom_edges = { 56, 57, 58, 59, 60, 61, 62, 63 };

bool is_edge_index(int i) { 
    return left_edges.contains(i) || right_edges.contains(i) || top_edges.contains(i) || bottom_edges.contains(i);
}

int Piece::color() { 
    return color_mask & this->data;
}

int Piece::type() { 
    return piece_mask & this->data;
}

std::string Piece::name() { 
    std::string name = ""; 

    switch(this->color()) { 
        case PieceType::WHITE:
            name.append("White ");
            break;
        case PieceType::BLACK: 
            name.append("Black ");
            break;
        default:
            name.append("wat");
            break;
    }

    switch (this->type()) { 
        case PieceType::PAWN: 
            name.append("Pawn");
            break;
        case PieceType::BISHOP: 
            name.append("Bishop");
            break;
        case PieceType::KNIGHT: 
            name.append("Horse");
            break;
        case PieceType::ROOK: 
            name.append("Rook");
            break;
        case PieceType::QUEEN: 
            name.append("Queen");
            break;
        case PieceType::KING: 
            name.append("King");
            break;
        default: 
            name.append("wat");
            break;
    }

    return name;
}

void calculate_pawn_vision(Piece& self, int index) { 
    self.vision.clear();
    int left, right, advance; 

    if (self.color() == PieceType::WHITE) { 
        left = index - 9;
        right = index - 7;
        advance = index - 8;
    } else { 
        right = index + 9;
        left = index + 7;
        advance = index + 8;
    }

    if (!left_edges.contains(advance)) { 
        self.vision.insert(left);
    }

    if (!right_edges.contains(advance)) { 
        self.vision.insert(right);
    }

    self.vision.insert(advance);
}

void calculate_rook_vision(Piece& self, int index) { 
    self.vision.clear();

    bool cast_left = !left_edges.contains(index);
    bool cast_right = !right_edges.contains(index);
    bool cast_up = !top_edges.contains(index);
    bool cast_down = !bottom_edges.contains(index);

    int left_current = index - 1;
    if (cast_left) {
        while (!left_edges.contains(left_current)) { 
            self.vision.insert(left_current); 
            left_current -= 1;
        }

        self.vision.insert(left_current);
    }

    int right_current = index + 1;
    if (cast_right) {
        while (!right_edges.contains(right_current)) { 
            self.vision.insert(right_current); 
            right_current += 1;
        }

        self.vision.insert(right_current);
    }

    int up_current = index - 8;
    if (cast_up) {
        while (!top_edges.contains(up_current)) { 
            self.vision.insert(up_current); 
            up_current -= 8;
        }

        self.vision.insert(up_current);
    }

    int down_current = index + 8;
    if (cast_down) {
        while (!bottom_edges.contains(down_current)) { 
            self.vision.insert(down_current); 
            down_current += 8;
        }

        self.vision.insert(down_current);
    }
}

void calculate_bishop_vision(Piece& self, int index) { 
    self.vision.clear();

    bool on_left_edge = left_edges.contains(index);
    bool on_right_edge = right_edges.contains(index);
    bool on_top_edge = top_edges.contains(index);
    bool on_bottom_edge = bottom_edges.contains(index);

    // cast ray up-left
    if (!on_left_edge && !on_top_edge) { 
        int current = index + Direction::NORTH_WEST; 
        while (!left_edges.contains(current) && !top_edges.contains(current)) { 
            self.vision.insert(current);
            current += Direction::NORTH_WEST;
        }

        self.vision.insert(current);
    }

    // cast ray up-right
    if (!on_right_edge && !on_top_edge) { 
        int current = index + Direction::NORTH_EAST; 
        while (!right_edges.contains(current) && !top_edges.contains(current)) { 
            self.vision.insert(current);
            current += Direction::NORTH_EAST;
        }

        self.vision.insert(current);
    }

    // cast ray down-left
    if (!on_left_edge && !on_bottom_edge) { 
        int current = index + 7; 
        while (!left_edges.contains(current) && !bottom_edges.contains(current)) { 
            self.vision.insert(current);
            current += 7;
        }

        self.vision.insert(current);
    }

    // cast ray down-right
    if (!on_right_edge && !on_bottom_edge) { 
        int current = index + 9; 
        while (!right_edges.contains(current) && !bottom_edges.contains(current)) { 
            self.vision.insert(current);
            current += 9;
        }

        self.vision.insert(current);
    }
}

void calculate_horsy_vision(Piece& self, int index) { 
    self.vision.clear();
    
    // TODO figure out how horses move.
}

void calculate_queen_vision(Piece& self, int index) { 
    self.vision.clear();
    std::set<int> bishop_vision; 
    
    calculate_bishop_vision(self, index);
    bishop_vision.merge(self.vision);
    
    calculate_rook_vision(self, index);
    self.vision.merge(bishop_vision);
}

void calculate_king_vision(Piece& self, int index) { 
    self.vision.clear();

    bool on_left_edge = left_edges.contains(index);
    bool on_right_edge = right_edges.contains(index);
    bool on_top_edge = top_edges.contains(index);
    bool on_bottom_edge = bottom_edges.contains(index);

    if (!on_left_edge) self.vision.insert(index - 1);
    if (!on_right_edge) self.vision.insert(index + 1);
    if (!on_bottom_edge) self.vision.insert(index + 8);
    if (!on_top_edge) self.vision.insert(index - 8);

    if (!on_left_edge && !on_top_edge) self.vision.insert(index - 9);
    if (!on_left_edge && !on_bottom_edge) self.vision.insert(index + 7);
    if (!on_right_edge && !on_top_edge) self.vision.insert(index - 7);
    if (!on_right_edge && !on_bottom_edge) self.vision.insert(index + 9);
}
