#include "engine.hpp"

#include <thread> 
#include <chrono> 
#include <cstdlib> 
#include <unordered_map>
#include <utility>
#include <time.h> 

#include "engine_state.hpp" 

std::unique_ptr<EngineState> state; 

std::pair<int, int> generate_random_move() { 
    EngineMovesMap moves = state->get_moves_for_active_color();

    auto move_map_iter = moves.begin(); 
    std::advance(move_map_iter, rand() % moves.size()); 
    
    int move_start = move_map_iter->first; 
    std::set<int> target_set = move_map_iter->second; 
    
    auto target_set_iter = target_set.begin(); 
    std::advance(target_set_iter, rand() % target_set.size());
    int move_target = *target_set_iter; 
    
    return std::pair<int, int>(move_start, move_target);
}

void engine_main(std::shared_ptr<Board> board) { 
    state = EngineState::init(board); 
    
    srand(time(0));

    while (true) { 
        int computer_color = PieceType::BLACK; // TODO parameterize this, somehow. Maybe through UI?

        if (board->active_color == computer_color) { 
            std::pair<int, int> move = generate_random_move(); 
            board->move_piece(move.first, move.second); 
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
}

