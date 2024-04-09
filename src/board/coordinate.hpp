#pragma once 

class Coordinate { 
public:
    int rank;
    int file; 

    Coordinate(int rank, int file) : rank(rank), file(file) {} 

    static Coordinate from_index(int);
    int get_index(); 
}; 