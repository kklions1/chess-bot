#include "piece.hpp"

int color_mask = 0b11000;
int piece_mask = 0b00111;

std::set<int> edges = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 23, 24,
     31, 32, 39, 40, 47, 48, 55, 56, 57, 58, 59, 60, 61, 62, 63 }; 

std::set<int> left_edges = { 0, 8, 16, 24, 32, 40, 48, 56 };
std::set<int> right_edges = { 7, 15, 23, 31, 39, 47, 55, 63 };
std::set<int> top_edges = { 0, 1, 2, 3, 4, 5, 6, 7 };
std::set<int> bottom_edges = { 56, 57, 58, 59, 60, 61, 62, 63 };

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
        // self.vision.push_back(index - 9); // left  
        // self.vision.push_back(index - 7); // right 
        // self.vision.push_back(index - 8); // forward
    } else { 
        right = index + 9;
        left = index + 7;
        advance = index + 8;

        // self.vision.push_back(index + 9); 
        // self.vision.push_back(index + 7);
        // self.vision.push_back(index + 8);
    }


    if (!left_edges.contains(advance)) { 
        self.vision.push_back(left);
    }

    if (!right_edges.contains(advance)) { 
        self.vision.push_back(right);
    }

    self.vision.push_back(advance);

}

