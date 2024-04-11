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

void cpu_move(Board* board) { 
    std::this_thread::sleep_for(std::chrono::seconds(5));
    auto result = board->move_piece(11, 19);
    std::cout << "Move made: " << result << std::endl;
}

int main(int argc, char** argv) {
    std::vector<std::string> program_args; 
    bool draw_gui = true; 

    if (argc > 1) { 
        for (int i = 1; i < argc; ++i) { 
            program_args.push_back(argv[i]);
        }
        
        draw_gui = std::find(program_args.begin(), program_args.end(), "--no-gui") == program_args.end(); // if flag no-gui flag is found, this will return false, and not draw the window
    }

    // std::string starting_fen = "rn1q1rk1/pp2ppbp/3p1np1/2p2b2/3P1B2/1QP1PN2/PP1NBPPP/R3K2R b KQ - 1 8";
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    auto game_board = std::make_unique<Board>();
    parse_fen_string(starting_fen, *game_board);
    game_board->generate_legal_moves();

    
    print_index();
   
    std::thread engine_thread(cpu_move, game_board.get());

    if (draw_gui) { 
        gui_main(game_board.get());
    } else { 
        main_window.close();
    }

    return 0; 
}