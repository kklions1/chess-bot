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

void debug_print_vector(const sf::Vector2f& vec) {
    std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}

std::vector<std::unique_ptr<PieceSprite>> init_sprites(Board* board, const TextureMap& texture_map) {
    std::vector<std::unique_ptr<PieceSprite>> result;
    for (int i = 0; i < 64; ++i) { 
        auto piece = board->board[i];
        if (piece->data != PieceType::EMPTY) { 
            result.push_back(PieceSprite::board_sprite(texture_map.at(piece->data), piece, calculate_position(i)));
        }
    }

    return result;
}

// Calculates the position for a sprite based on its index on the board
sf::Vector2f calculate_position(int index) { 
    int file = index / 8;
    int rank = index % 8; 

    return sf::Vector2f(rank * 100, file * 100);
}

// Calculates the index in the board for a sprite based on its position 
int calculate_index(const sf::Vector2i& pos) { 
    int x = (pos.x / 100); 
    int y = (pos.y / 100); 

    return (y * 8) + x; 
}

int calculate_index(const sf::Vector2f& pos) { 
    // convert position into simple (x, y) coordinates 
    int x = (pos.x / 100); 
    int y = (pos.y / 100); 

    return (y * 8) + x; 
}

PieceSprite* get_piece_at_position(const sf::Vector2i& pos, const std::vector<std::unique_ptr<PieceSprite>>& sprites) {
    for (int i = 0; i < sprites.size(); ++i) { 
        if (sprites[i]->shape.getGlobalBounds().contains(sf::Vector2f(pos))) { 
            return sprites[i].get(); 
        }
    }

    return nullptr;
}

int get_sprite_index_for_board_index(int index, const std::vector<std::unique_ptr<PieceSprite>>& sprites) {
    sf::Vector2f pos = calculate_position(index);

    for (int i = 0; i < sprites.size(); ++i) { 
        if (sprites[i]->shape.getPosition() == sf::Vector2f(pos)) { 
            return i; 
        }
    }

    return -1;
}

void snap_piece_to_square(const sf::Vector2f& mouse_pos, PieceSprite* sprite) { 
    if (sprite == nullptr) return;

    sprite->setPosition(mouse_pos);
}

void show_moves_for_piece(const PieceSprite* vision_target, std::vector<sf::CircleShape>& legal_move_indicatior, int active_color) { 
    if (vision_target == nullptr) return;

    if (vision_target->piece->color() != active_color) return;

    std::set<int> vision = vision_target->piece->vision; 
    for (int i : vision) { 
        sf::Vector2f position = calculate_position(i);
        sf::CircleShape indicator(50.0f);
        indicator.setFillColor(sf::Color(0, 0, 0, 100));
        indicator.setPosition(position);

        legal_move_indicatior.push_back(indicator);
    }
}

void gui_main(std::shared_ptr<Board> board) {
    auto texture_map = init_textures();
    auto squares = init_squares();
    auto pieces = init_sprites(board.get(), texture_map);
    std::vector<sf::CircleShape> legal_move_indicator;
    PieceSprite* drag_target;
    PieceSprite* vision_target;
    int move_target_starting_index;
    int move_target_index;
    bool is_dragging = false;
    int current_halfmove_clock = board->halfmove_clock; 

    while (main_window.isOpen()) {
        sf::Event event;
        while (main_window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: { 
                    main_window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed: { 
                    sf::Vector2i mouse_pos(event.mouseButton.x, event.mouseButton.y);
                    move_target_starting_index = calculate_index(mouse_pos);
                    auto sprite_ptr = get_piece_at_position(mouse_pos, pieces);
                    drag_target = sprite_ptr; 
                    vision_target = sprite_ptr;
                    
                    legal_move_indicator.clear();
                    show_moves_for_piece(vision_target, legal_move_indicator, board->active_color);
                    is_dragging = true;
                    break;
                }
                case sf::Event::MouseButtonReleased: { 
                    is_dragging = false;
                    if (drag_target == nullptr) break;

                    sf::Vector2f mouse_pos(sf::Mouse::getPosition(main_window));
                    
                    switch (board->move_piece(move_target_starting_index, move_target_index = calculate_index(mouse_pos))) { 
                        case MoveType::NO_MOVE: { 
                            // Nothing happened, piece should go back to where it came from
                            drag_target->setPosition(calculate_position(move_target_starting_index));
                            break;
                        }
                        case MoveType::MOVE: { 
                            // The piece moved. Snap it to the square that it should have moved to.
                            // Might require some double checking. things are kinda buggy
                            drag_target->setPosition(calculate_position(move_target_index));
                            legal_move_indicator.clear();
                            break;
                        }
                        case MoveType::CAPTURE: { 
                            // TODO test to make sure this still works.
                            if (int sprite_index = get_sprite_index_for_board_index(move_target_index, pieces) != -1) { 
                                pieces.erase(pieces.begin() + sprite_index);
                            }

                            drag_target->setPosition(mouse_pos);
                            legal_move_indicator.clear();
                            break;
                        }
                    }
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

        if (current_halfmove_clock != board->halfmove_clock) { 
            pieces = init_sprites(board.get(), texture_map);
            current_halfmove_clock = board->halfmove_clock;
        }

        if (is_dragging && drag_target != nullptr) {
            sf::Vector2f mouse_pos(sf::Mouse::getPosition(main_window));
            mouse_pos.x -= 50.0f;
            mouse_pos.y -= 50.0f;
            drag_target->shape.setPosition(mouse_pos);
        }

        for (auto const s : squares) {
            main_window.draw(s);
        }

        for (auto const& p : pieces) { 
            main_window.draw(p->shape);
        }

        for (auto const move : legal_move_indicator) { 
            main_window.draw(move);
        }

        if (board->game_end_state != GameEndState::PLAYING) { 
            sf::Font font; 
            font.loadFromFile("../assets/Roboto-Regular.ttf");

            sf::RectangleShape popup_window(sf::Vector2f(200.0, 50.0)); 
            popup_window.setPosition(sf::Vector2f(350.0, 350.0)); 
            popup_window.setFillColor(sf::Color::White);
            popup_window.setOutlineColor(sf::Color::Black);
            popup_window.setOutlineThickness(2.0);

            sf::Text game_over_text; 
            game_over_text.setFont(font); 
            
            if (board->game_end_state == GameEndState::CHECKMATE) { 
                if (board->active_color == PieceType::WHITE) { 
                    game_over_text.setString("Black Wins!");
                } else { 
                    game_over_text.setString("White Wins!");
                }
            }

            if (board->game_end_state == GameEndState::STALEMATE) { 
                game_over_text.setString("Stalemate :(");
            }

            game_over_text.setFillColor(sf::Color::Red);
            game_over_text.setPosition(sf::Vector2f(350.0, 350.0));

            main_window.draw(popup_window);
            main_window.draw(game_over_text);
        }

        main_window.display();
    }
}
