#include <iostream> 
#include <memory>
#include <string> 

#include "gui/piece_sprite.hpp"
#include "common/common_structs.hpp"
#include "board/board.hpp"
#include "gui/gui_main.hpp"

PieceSprite create_spite(std::shared_ptr<sf::Texture> texture) { 
    auto sprite = PieceSprite(); 
    sprite.texture = texture;
    sprite.shape = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
    sprite.shape.setTexture(sprite.texture.get());
    sprite.shape.setFillColor(sf::Color(255, 255, 255, 0));
    sprite.shape.setScale(sf::Vector2f(0.75f, 0.75f));
    
    return sprite;
}

int main() { 
    std::string test_fen = "rn1q1rk1/pp2ppbp/3p1np1/2p2b2/3P1B2/1QP1PN2/PP1NBPPP/R3K2R b KQ - 1 8"; 
    std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    auto game_board = std::make_unique<Board>();
    parse_fen_string(test_fen, *game_board);
    // print_board(*game_board);

    auto starting_position = std::make_unique<Board>();
    parse_fen_string(starting_fen, *starting_position);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess");

    auto texture_map = init_textures();
    auto squares = init_squares();

    std::vector<PieceSprite> pieces; 

    for (int i = 0; i < 64; ++i) { 
        int piece = game_board->board[i];
        if (piece != Piece::EMPTY) { 
            PieceSprite sprite = create_sprite(texture_map[piece]);
            // sprite.shape.setPosition(squares[i].getPosition());
            // pieces.push_back(sprite);
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
                    std::cout << "Button pressed" << std::endl;
                    break;
                case sf::Event::MouseButtonReleased:  
                    std::cout << "Button Released" << std::endl;
                    break;
                default: 
                    break;
            }
        }

        window.clear(sf::Color::Green); // If green is showing, its probably because of some rendering problem

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