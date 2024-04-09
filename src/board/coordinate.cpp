#include "coordinate.hpp"

Coordinate Coordinate::from_index(int index) {
    return Coordinate(index / 8, index % 8);
}

int Coordinate::get_index() { 
    return (this->rank * 8) + this->file;
} 