#include "engine_state.hpp" 

EngineMovesMap EngineState::get_moves_by_color(int color) { 
   std::unordered_map<int, std::set<int>> result; 

    for (int i = 0; i < 64; ++i) { 
        Piece* current = this->current_position->board[i].get();
        if (this->current_position->active_color != current->color()) { 
            continue; 
        }

        for (int target : current->vision) { 
            result.insert({ i, current->vision });
        }
    } 

    return result; 
}

EngineMovesMap EngineState::get_moves_for_active_color() { 
    return get_moves_by_color(this->current_position->active_color);
}

std::unique_ptr<EngineState> EngineState::init(std::shared_ptr<Board> board) { 
    auto state = std::make_unique<EngineState>(); 
    state->current_position = board; 

    return state; 
}