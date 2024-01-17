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