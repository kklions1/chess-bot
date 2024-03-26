#include "move_result.hpp"

std::unique_ptr<MoveResult> MoveResult::create(int start, int target, Board& initial) {
    auto result = initial.copy();
    MoveType type = result->move_piece(start, target); 
    if (type != MoveType::NO_MOVE) { 
        // return std::make_unique<MoveResult>(); 
        return nullptr;
    }

    return nullptr; 
}