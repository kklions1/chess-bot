#include <iostream> 
#include <memory>
#include <string> 
#include <thread> 

#include "gui/piece_sprite.hpp"
#include "common/common_structs.hpp"
#include "board/board.hpp"
#include "gui/gui_main.hpp"
#include "board/engine.hpp"

int main() { 
    // std::string test_fen = "rn1q1rk1/pp2ppbp/3p1np1/2p2b2/3P1B2/1QP1PN2/PP1NBPPP/R3K2R b KQ - 1 8"; 
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    auto game_board = std::make_unique<Board>();
    parse_fen_string(starting_fen, *game_board);

    // Might put these into some kind of state object for the UI.
    auto texture_map = init_textures();
    auto squares = init_squares();
    auto pieces = init_sprites(*game_board, texture_map); 
    PieceSprite* drag_target; 
    bool is_dragging = false;

    generate_legal_moves(*game_board);
    
    while (main_window.isOpen()) { 
        sf::Event event;
        while (main_window.pollEvent(event)) { 
            switch (event.type) { 
                case sf::Event::Closed:
                    main_window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    // std::cout << "Button pressed: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
                    drag_target = get_piece_at_position(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), pieces); 
                    is_dragging = true;
                    break;
                case sf::Event::MouseButtonReleased:  
                    // std::cout << "Button released: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
                    snap_piece_to_square(sf::Vector2f(sf::Mouse::getPosition(main_window)), drag_target);
                    is_dragging = false;
                    drag_target = nullptr;
                    break;
                default: 
                    break;
            }
        }

        main_window.clear(sf::Color::Green); // If green is showing, its probably because of some rendering problem

        if (is_dragging && drag_target != nullptr) { 
            sf::Vector2f mouse_pos(sf::Mouse::getPosition(main_window));
            mouse_pos.x -= 50.0f;
            mouse_pos.y -= 50.0f;
            drag_target->shape.setPosition(mouse_pos);
        }

        for (auto s : squares) { 
            main_window.draw(s);
        }

        for (auto p : pieces) { 
            main_window.draw(p.shape);
        }

        main_window.display();
    }

    gui_main(*game_board);

    return 0; 
}