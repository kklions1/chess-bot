#include "gui_main.hpp"

#include <iostream>

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

std::unordered_map<int, std::shared_ptr<sf::Texture>> init_textures() { 
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

    std::unordered_map<int, std::shared_ptr<sf::Texture>> texture_map = {
        { Piece::BLACK | Piece::PAWN, black_pawn },
        { Piece::BLACK | Piece::ROOK, black_rook },
        { Piece::BLACK | Piece::KNIGHT, black_horse },
        { Piece::BLACK | Piece::BISHOP, black_bishop },
        { Piece::BLACK | Piece::QUEEN, black_queen },
        { Piece::BLACK | Piece::KING, black_king },
        { Piece::WHITE | Piece::PAWN, white_pawn },
        { Piece::WHITE | Piece::ROOK, white_rook },
        { Piece::WHITE | Piece::KNIGHT, white_horse },
        { Piece::WHITE | Piece::BISHOP, white_bishop },
        { Piece::WHITE | Piece::QUEEN, white_queen },
        { Piece::WHITE | Piece::KING, white_king },
    };

    return texture_map; 
}

PieceSprite* get_piece_at_position(const sf::Vector2i& click_pos, std::vector<PieceSprite>& sprites) {
    for (int i = 0; i < sprites.size(); ++i) { 
        if (sprites[i].shape.getGlobalBounds().contains(sf::Vector2f(click_pos))) { 
            return &sprites[i]; 
        }
    }

    std::cout << "No dice\n";
    return nullptr;
}

sf::Vector2f normalize_to_corner(const sf::Vector2f& pos) { 
    debug_print_vector(sf::Vector2f((((int) pos.x / 100) * 100), ((int) (pos.y / 100) * 100)));
    return sf::Vector2f((((int) pos.x / 100) * 100), ((int) (pos.y / 100) * 100 ));
}

void snap_piece_to_square(const sf::Vector2f& mouse_pos, PieceSprite* sprite) { 
    if (sprite != nullptr) { 
        sprite->shape.setPosition(sf::Vector2f(normalize_to_corner(mouse_pos)));
    }
}

void debug_print_vector(const sf::Vector2f& vec) { 
    std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}