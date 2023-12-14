#include "piece_sprite.hpp"

std::unique_ptr<Square> create_square(std::shared_ptr<sf::Texture> texture) { 
    auto square = std::make_unique<Square>(); 
    square->piece_texture = texture;
    square->sprite = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
    square->sprite.setTexture(square->piece_texture.get());
    square->sprite.setFillColor(sf::Color(255, 255, 255, 0));
    square->sprite.setScale(sf::Vector2f(0.75f, 0.75f));
    
    return square;
}