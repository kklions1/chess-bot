#pragma once 

#include <SFML/Graphics.hpp>

#include "../common/common_structs.hpp"

typedef struct PieceSprite { 
    std::shared_ptr<sf::Texture> piece_texture;
    sf::RectangleShape sprite;
} Square; 

std::unique_ptr<PieceSprite> create_square(std::shared_ptr<sf::Texture>);