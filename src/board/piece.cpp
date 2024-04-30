#include "piece.hpp"

#include <iostream>

int color_mask = 0b11000;
int piece_mask = 0b00111;

Piece::Piece() { 
    this->data = PieceType::EMPTY;
    this->vision = std::set<int>();
}

std::shared_ptr<Piece> Piece::clone() { 
    auto copied = std::make_shared<Piece>();
    copied->data = this->data;
    copied->vision.insert(this->vision.begin(), this->vision.end());
    
    return copied;
}

int Piece::color() { 
    return color_mask & this->data;
}

int Piece::type() { 
    return piece_mask & this->data;
}

std::shared_ptr<Piece> Piece::make_empty() { 
    return std::make_shared<Piece>();
}

std::string colorName(int color) { 
    if (color == PieceType::WHITE) { 
        return "White";
    } else { 
        return "Black";
    }
}

std::string Piece::name() { 
    if (this->data == PieceType::EMPTY) { return "Empty"; }

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

void move(Piece* moving, Piece* destination) { 
    if (moving == nullptr || destination == nullptr) {
        throw std::runtime_error("cannot move piece to or from a nullptr"); 
    }

    destination->data = moving->data; 
    moving->data = PieceType::EMPTY; 

    destination->vision.clear();
    moving->vision.clear();
}