#include <iostream> 
#include <memory>
#include <string> 

#include "gui/piece_sprite.hpp"
#include "common/common_structs.hpp"
#include "board/board.hpp"
#include "gui/gui_main.hpp"

int main() { 
    // std::string test_fen = "rn1q1rk1/pp2ppbp/3p1np1/2p2b2/3P1B2/1QP1PN2/PP1NBPPP/R3K2R b KQ - 1 8"; 
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    auto game_board = std::make_unique<Board>();
    parse_fen_string(starting_fen, *game_board);

    gui_main(*game_board);

    return 0; 
}