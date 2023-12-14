#include <iostream> 
#include <memory>
#include <string> 

#include "common/common_structs.hpp"
#include "board/board.hpp"
#include "gui/gui_main.hpp"

int main() { 
    // std::string test_fen = "rn1q1rk1/pp2ppbp/3p1np1/2p2b2/3P1B2/1QP1PN2/PP1NBPPP/R3K2R b KQ - 1 8"; 
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // auto game_board = std::make_unique<Board>();
    // parse_fen_string(test_fen, *game_board);
    // print_board(*game_board);

    auto starting_position = std::make_unique<Board>();
    parse_fen_string(starting_fen, *starting_position);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");

    auto texture_map = init_textures();
    auto squares = generate_squares();

    for (int i = 0; i < 64; ++i) { 
        int piece = starting_position->board[i];
        squares[i].setTexture(texture_map[piece].get());
    }

    while (window.isOpen()) { 
        sf::Event event;
        while (window.pollEvent(event)) { 
            if (event.type == sf::Event::Closed) { 
                window.close();
            }
        }

        window.clear(sf::Color::Green); // If green is showing, its probably because of some rendering problem

        for (auto s : squares) { 
            window.draw(s);
        }

        window.display();
    }

    return 0; 
}