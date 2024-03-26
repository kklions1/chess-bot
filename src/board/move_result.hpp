#pragma once 

#include "board.hpp"
#include "enum.hpp"

#include <memory> 

class MoveResult { 
private: 
    std::shared_ptr<Board> result_position; 
    MoveType type; 

    

public: 
    // MoveResult(int, int, Board&);

    static std::unique_ptr<MoveResult> create(int, int, Board&);

    Board* getResult(); 
    MoveResult(MoveType, std::shared_ptr<Board>);


};