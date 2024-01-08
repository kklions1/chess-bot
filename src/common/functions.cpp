#include "functions.hpp"

int color_mask = 0b11000;
int piece_mask = 0b00111;

bool is_white(int piece) { 
    return piece & color_mask == Piece::WHITE;
}

bool is_black(int piece) { 
    return piece & color_mask;
} 

bool is_pawn(int piece) { 
    return piece & piece_mask == Piece::PAWN;
}

bool is_horse(int piece) { 
    return false;
}

bool is_rook(int piece) { 
    return false;
}

bool is_bishop(int piece) { 
    return false;
}

bool is_queen(int piece) { 
    return false;
}

bool is_king(int piece) { 
    return false;
}
