#include "piece.hpp"

int color_mask = 0b11000;
int piece_mask = 0b00111;

int Piece::color() { 
    return color_mask & this->data;
}

int Piece::type() { 
    return piece_mask & this->data;
}

void calculate_pawn_vision(Piece& self, int index) { 
    int left, right, advance; 

    if (self.color() == PieceType::WHITE) { 
        self.vision.push_back(index - 9); // left  
        self.vision.push_back(index - 7); // right 
        self.vision.push_back(index - 8); // forward
    } else { 
        self.vision.push_back(index + 9); 
        self.vision.push_back(index + 7);
        self.vision.push_back(index + 8);
    }
}

