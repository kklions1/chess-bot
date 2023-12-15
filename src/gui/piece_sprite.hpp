#pragma once 

#include <SFML/Graphics.hpp>
#include <memory>

#include "../common/common_structs.hpp"

typedef struct PieceSprite { 
    std::shared_ptr<sf::Texture> texture;
    sf::RectangleShape shape;
} PieceSprite; 

PieceSprite create_sprite(std::shared_ptr<sf::Texture>);