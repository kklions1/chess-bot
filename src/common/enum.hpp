#pragma once 

typedef enum PieceType { 
    EMPTY = 0, 
    PAWN = 1, 
    KNIGHT = 2, 
    BISHOP  = 3, 
    ROOK = 4, 
    QUEEN = 5, 
    KING = 6, 

    WHITE = 8,
    BLACK = 16 
} PieceType; 

typedef enum Direction { 
    NORTH = -8, 
    SOUTH = 8, 
    EAST = 1, 
    WEST = -1, 
    NORTH_EAST = -7, 
    NORTH_WEST = -9, 
    SOUTH_EAST = 9, 
    SOUTH_WEST = 7
} Direction;

typedef enum MoveResult { 
    NO_MOVE, 
    MOVE,
    CAPTURE 
} MoveResult; 