#pragma once 

#include <vector>
#include <set>

extern int piece_mask;
extern int color_mask;

extern std::set<int> left_edges; // A File 
extern std::set<int> right_edges; // H File 
extern std::set<int> top_edges; // 8th Rank
extern std::set<int> bottom_edges; // 1st Rank

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

typedef struct Piece { 
    int data; 
    std::vector<int> vision; 

    void (*calc_vision)(Piece&, int); 

    int color();
    int type();

    Piece(): data(PieceType::EMPTY), vision(std::vector<int>()), calc_vision([](Piece& self, int index) { /* no-op */ }) {}
} Piece; 

typedef std::shared_ptr<Piece> Piece_ptr;

void calculate_pawn_vision(Piece&, int);
void calculate_rook_vision(Piece&, int);
void calculate_bishop_vision(Piece&, int);

