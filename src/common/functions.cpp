#include "functions.hpp"

int color_mask = 0b11000;
int piece_mask = 0b00111;

bool is_white(int piece) { 
    return piece & color_mask == PieceType::WHITE;
}

bool is_black(int piece) { 
    return piece & color_mask;
} 

bool is_pawn(int piece) { 
    return piece & piece_mask == PieceType::PAWN;
}

bool is_horse(int piece) { 
    return piece & piece_mask == PieceType::KNIGHT;
}

bool is_rook(int piece) { 
    return piece & piece_mask == PieceType::ROOK;
}

bool is_bishop(int piece) { 
    return piece & piece_mask == PieceType::BISHOP;
}

bool is_queen(int piece) { 
    return piece & piece_mask == PieceType::QUEEN;
}

bool is_king(int piece) { 
    return piece & piece_mask == PieceType::KING;
}
