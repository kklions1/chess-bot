#include <iostream> 
#include <memory>
#include <string> 
#include <thread> 
#include <chrono> 
#include <algorithm>
#include <unordered_map>

#include "gui/piece_sprite.hpp"
#include "board/board.hpp"
#include "gui/gui_main.hpp"
#include "engine/engine.hpp"

int main(int argc, char** argv) {
    // std::string starting_fen = "rn1q1rk1/pp2ppbp/3p1np1/2p2b2/3P1B2/1QP1PN2/PP1NBPPP/R3K2R b KQ - 1 8";
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    auto game_board = std::make_shared<Board>();
    parse_fen_string(starting_fen, *game_board);
    game_board->generate_legal_moves();

    
    print_index();
   
    std::thread engine_thread(engine_main, game_board);

    gui_main(game_board);


    return 0; 
}