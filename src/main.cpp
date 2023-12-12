#include <iostream> 
#include <memory>
#include <string> 

#include "common/common_structs.hpp"
#include "board/board.hpp"

int main() { 
    std::string test_fen = "rn1q1rk1/pp2ppbp/3p1np1/2p2b2/3P1B2/1QP1PN2/PP1NBPPP/R3K2R b KQ - 1 8"; 

    Board* game_board = new Board(); 


    parse_fen_string(test_fen, *game_board);
    // print_board(*game_board);
    // std::string generated_fen = generate_fen_string(*game_board);

    delete game_board; 

    return 0; 
}