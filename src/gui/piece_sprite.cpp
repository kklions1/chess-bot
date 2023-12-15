#include "piece_sprite.hpp"

PieceSprite create_sprite(std::shared_ptr<sf::Texture> texture) { 
    auto sprite = PieceSprite(); 
    sprite.texture = texture;
    sprite.shape = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
    sprite.shape.setTexture(sprite.texture.get());
    sprite.shape.setFillColor(sf::Color(255, 255, 255, 0));
    sprite.shape.setScale(sf::Vector2f(0.75f, 0.75f));
    
    return sprite;
}