#include "gui_main.hpp"

sf::RenderWindow main_window = sf::RenderWindow(sf::VideoMode(800, 800), "Chess");

std::vector<sf::RectangleShape> init_squares() {
    float current_x = 0.0f;
    float current_y = 0.0f;
    bool color_alternator = true;
    std::vector<sf::RectangleShape> squares;

    for (int file = 0; file < 8; file++) {
        for (int rank = 0; rank < 8; rank++) {
            if (color_alternator) {
                sf::RectangleShape light_square(sf::Vector2f(100.0f, 100.0f));
                light_square.setFillColor(sf::Color::White);
                light_square.setPosition(sf::Vector2f(rank * 100, file * 100));

                squares.push_back(light_square);
            } else {
                sf::RectangleShape dark_square(sf::Vector2f(100.0f, 100.0f));
                dark_square.setFillColor(sf::Color(171, 171, 255));
                dark_square.setPosition(sf::Vector2f(rank * 100, file * 100));

                squares.push_back(dark_square);
            }

            color_alternator = !color_alternator;
        }
        color_alternator = !color_alternator;
    }

    return squares;
}

TextureMap init_textures() { 
    auto black_pawn = std::make_shared<sf::Texture>();
    auto black_rook = std::make_shared<sf::Texture>();
    auto black_horse = std::make_shared<sf::Texture>();
    auto black_bishop = std::make_shared<sf::Texture>();
    auto black_queen = std::make_shared<sf::Texture>();
    auto black_king = std::make_shared<sf::Texture>();

    auto white_pawn = std::make_shared<sf::Texture>();
    auto white_rook = std::make_shared<sf::Texture>();
    auto white_horse = std::make_shared<sf::Texture>();
    auto white_bishop = std::make_shared<sf::Texture>();
    auto white_queen = std::make_shared<sf::Texture>();
    auto white_king = std::make_shared<sf::Texture>();

    // Black Pieces 
    if (!black_pawn->loadFromFile("../assets/black-pawn.png")) throw std::runtime_error("file not found: black-pawn.png");
    
    if (!black_rook->loadFromFile("../assets/black-rook.png")) throw std::runtime_error("file not found: black-rook.png");

    if (!black_horse->loadFromFile("../assets/black-horse.png")) throw std::runtime_error("file not found: black-horse.png");

    if (!black_bishop->loadFromFile("../assets/black-bishop.png")) throw std::runtime_error("file not found: black-bishop.png");
    
    if (!black_queen->loadFromFile("../assets/black-queen.png")) throw std::runtime_error("file not found: black-queen.png");
    
    if (!black_king->loadFromFile("../assets/black-king.png")) throw std::runtime_error("file not found: black-king.png");

    // White Pieces
    if (!white_pawn->loadFromFile("../assets/white-pawn.png")) throw std::runtime_error("file not found: white-pawn.png");

    if (!white_rook->loadFromFile("../assets/white-rook.png")) throw std::runtime_error("file not found: white-rook.png");

    if (!white_horse->loadFromFile("../assets/white-horse.png")) throw std::runtime_error("file not found: white-horse.png");

    if (!white_bishop->loadFromFile("../assets/white-bishop.png")) throw std::runtime_error("file not found: white-bishop.png");
    
    if (!white_queen->loadFromFile("../assets/white-queen.png")) throw std::runtime_error("file not found: white-queen.png");
    
    if (!white_king->loadFromFile("../assets/white-king.png")) throw std::runtime_error("file not found: white-king.png");

    TextureMap texture_map = {
        { PieceType::BLACK | PieceType::PAWN, black_pawn },
        { PieceType::BLACK | PieceType::ROOK, black_rook },
        { PieceType::BLACK | PieceType::KNIGHT, black_horse },
        { PieceType::BLACK | PieceType::BISHOP, black_bishop },
        { PieceType::BLACK | PieceType::QUEEN, black_queen },
        { PieceType::BLACK | PieceType::KING, black_king },
        { PieceType::WHITE | PieceType::PAWN, white_pawn },
        { PieceType::WHITE | PieceType::ROOK, white_rook },
        { PieceType::WHITE | PieceType::KNIGHT, white_horse },
        { PieceType::WHITE | PieceType::BISHOP, white_bishop },
        { PieceType::WHITE | PieceType::QUEEN, white_queen },
        { PieceType::WHITE | PieceType::KING, white_king },
    };

    return texture_map;
}

sf::Vector2f normalize_to_corner(const sf::Vector2f& pos) {
    return sf::Vector2f((((int) pos.x / 100) * 100), ((int) (pos.y / 100) * 100));
}

void debug_print_vector(const sf::Vector2f& vec) {
    std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}

void show_moves_for_piece(PieceSprite* vision_target) { 

}

void gui_main(const Board& board) {
    auto texture_map = init_textures();
    auto squares = init_squares();
    auto pieces = init_sprites(board, texture_map);
    std::vector<sf::CircleShape> legal_move_indicator;
    PieceSprite* drag_target;
    PieceSprite* vision_target;
    bool is_dragging = false;

    while (main_window.isOpen()) {
        sf::Event event;
        while (main_window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: { 
                    main_window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed: { 
                    // std::cout << "Button pressed: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
                    PieceSprite* sprite_ptr = get_piece_at_position(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), pieces);
                    drag_target = sprite_ptr; 
                    vision_target = sprite_ptr; 
                    show_moves_for_piece(vision_target);
                    is_dragging = true;
                    break;
                }
                case sf::Event::MouseButtonReleased: { 
                    // std::cout << "Button released: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
                    snap_piece_to_square(sf::Vector2f(sf::Mouse::getPosition(main_window)), drag_target);
                    is_dragging = false;
                    drag_target = nullptr;
                    break;
                }
                case sf::Event::KeyPressed: { 
                    switch (event.key.code) { 
                        case sf::Keyboard::Backspace: { 
                            vision_target = nullptr;
                            legal_move_indicator.clear();
                            std::cout << "clear spaces!" << std::endl;
                            break;
                        }
                        default: { 
                            break;
                        }
                    }
                    break;
                }
                default: { 
                    break;
                }
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

        for (auto move : legal_move_indicator) { 
            main_window.draw(move);
        }

        main_window.display();
    }
}
