#pragma once 

#include "engine.hpp" 

#include <memory>
#include <unordered_map>
#include <set> 

typedef std::unordered_map<int, std::set<int>> EngineMovesMap;

class EngineState { 
private: 
    std::shared_ptr<Board> current_position; 

public:
    static std::unique_ptr<EngineState> init(std::shared_ptr<Board>);
    
    EngineMovesMap get_moves_by_color(int); // PieceType::BLACK or PieceType::WHITE
    EngineMovesMap get_moves_for_active_color(); 

}; 
