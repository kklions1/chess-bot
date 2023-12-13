#include <iostream> 
#include <memory>
#include <string> 

#include "common/common_structs.hpp"
#include "board/board.hpp"
#include "gui/gui_main.hpp"

int main() { 
    std::string test_fen = "rn1q1rk1/pp2ppbp/3p1np1/2p2b2/3P1B2/1QP1PN2/PP1NBPPP/R3K2R b KQ - 1 8"; 
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // auto game_board = std::make_unique<Board>();

    // parse_fen_string(test_fen, *game_board);
    // print_board(*game_board);
    // // std::string generated_fen = generate_fen_string(*game_board);

    // auto starting_position = std::make_unique<Board>();
    // parse_fen_string(starting_fen, *starting_position);
    // print_board(*starting_position);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");

    float current_x = 0.0f;
    float current_y = 0.0f;
    std::vector<sf::RectangleShape> squares; 
    bool alternator = false;

    for (int i = 0; i < 64; i++) { 
        if (alternator) { 
            std::cout << "coordiantes: x = " << current_x << " y = " << current_y << std::endl;
            squares.push_back(dark_square(current_x, current_y));
        } else { 
            squares.push_back(light_square(current_x, current_y));
        }

        if (i % 8 == 0 && i != 0) { 
            current_x = 0.0f;
            current_y += 100.0f;    
        } else { 
            current_x += 100.0f;
        }

        alternator = !alternator;
    }

    while (window.isOpen()) { 
        sf::Event event;
        while (window.pollEvent(event)) { 
            if (event.type == sf::Event::Closed) { 
                window.close();
            }

        }

        window.clear(sf::Color::Green);

        for (sf::RectangleShape r : squares) { 
            window.draw(r);
        }

        // sf::RectangleShape red(sf::Vector2f(100.0f, 100.0f));
        // red.setFillColor(sf::Color::Red);
        // red.setPosition(sf::Vector2f(0.0f, 100.0f));
        // window.draw(red);

        window.display();
    }

    return 0; 
}