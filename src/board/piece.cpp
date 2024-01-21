#include "piece.hpp"

#include <iostream>

int color_mask = 0b11000;
int piece_mask = 0b00111;

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