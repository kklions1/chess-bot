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

    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");

    // Might put these into some kind of state object for the UI.
    auto texture_map = init_textures();
    auto squares = init_squares();
    std::vector<PieceSprite> pieces; 
    PieceSprite* current_drag; 
    bool is_dragging = false;

    for (int i = 0; i < 64; ++i) { 
        int piece = game_board->board[i];
        if (piece != Piece::EMPTY) { 
            PieceSprite sprite = create_sprite(texture_map[piece]);
            sprite.shape.setPosition(calculate_position(i));
            sprite.piece = piece;
            pieces.push_back(sprite);
        }
    }
    
    while (window.isOpen()) { 
        sf::Event event;
        while (window.pollEvent(event)) { 
            switch (event.type) { 
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    std::cout << "Button pressed: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
                    // sf::Vector2i init_pos(event.mouseButton.x, event.mouseButton.y);
                    current_drag = get_piece_at_position(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), pieces); 
                    is_dragging = true;
                    break;
                case sf::Event::MouseButtonReleased:  
                    std::cout << "Button released: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
                    is_dragging = false;
                    current_drag = nullptr;
                    break;
                default: 
                    break;
            }
        }

        window.clear(sf::Color::Green); // If green is showing, its probably because of some rendering problem

        if (is_dragging && current_drag != nullptr) { 
            current_drag->shape.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
        }

        for (auto s : squares) { 
            window.draw(s);
        }

        for (auto p : pieces) { 
            window.draw(p.shape);
        }

        window.display();
    }

    return 0; 
}