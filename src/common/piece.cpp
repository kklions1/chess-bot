#include "piece.hpp"

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

void calculate_pawn_vision(Piece& self, int index) { 
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
        self.vision.push_back(left);
    }

    if (!right_edges.contains(advance)) { 
        self.vision.push_back(right);
    }

    self.vision.push_back(advance);
}

void calculate_rook_vision(Piece& self, int index) { 
    bool cast_left = !left_edges.contains(index);
    bool cast_right = !right_edges.contains(index);
    bool cast_up = !top_edges.contains(index);
    bool cast_down = !bottom_edges.contains(index);

    int left_current = index -1;
    if (cast_left) {
        while (!left_edges.contains(left_current)) { 
            self.vision.push_back(left_current); 
            left_current -= 1;
        }

        self.vision.push_back(left_current);
    }

    int right_current = index + 1;
    if (cast_right) {
        while (!right_edges.contains(right_current)) { 
            self.vision.push_back(right_current); 
            right_current += 1;
        }

        self.vision.push_back(right_current);
    }

    int up_current = index - 8;
    if (cast_up) {
        while (!top_edges.contains(up_current)) { 
            self.vision.push_back(up_current); 
            up_current -= 8;
        }

        self.vision.push_back(up_current);
    }

    int down_current = index + 8;
    if (cast_down) {
        while (!bottom_edges.contains(down_current)) { 
            self.vision.push_back(down_current); 
            down_current += 8;
        }

        self.vision.push_back(down_current);
    }
}

void calculate_bishop_vision(Piece& self, int index) { 
    bool on_left_edge = left_edges.contains(index);
    bool on_right_edge = right_edges.contains(index);
    bool on_top_edge = top_edges.contains(index);
    bool on_bottom_edge = bottom_edges.contains(index);

    

}
