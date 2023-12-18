#pragma once 

#include <SFML/Graphics.hpp>
#include <memory>

#include "../common/common_structs.hpp"

typedef struct PieceSprite { 
    std::shared_ptr<sf::Texture> texture;
    sf::RectangleShape shape;
    int piece;
} PieceSprite; 

PieceSprite create_sprite(std::shared_ptr<sf::Texture>);
sf::Vector2f calculate_position(int);
int calculate_index(const sf::Vector2f&);
// void snap_to_square(const sf::Vector2f&);