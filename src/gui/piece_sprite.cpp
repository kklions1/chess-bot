#include "piece_sprite.hpp"

PieceSprite create_sprite(std::shared_ptr<sf::Texture> texture) { 
    auto sprite = PieceSprite(); 
    sprite.texture = texture;
    sprite.shape = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
    sprite.shape.setTexture(sprite.texture.get());
    // sprite.shape.setFillColor(sf::Color(255, 255, 255, 0));
    // sprite.shape.setScale(sf::Vector2f(0.75f, 0.75f));
    
    return sprite;
}

sf::Vector2f calculate_position(int index) { 
    int file = index / 8;
    int rank = index % 8; 

    return sf::Vector2f(rank * 100, file * 100);
}

int calculate_index(const sf::Vector2f& pos) { 
    int x = pos.x / 100; 
    int y = pos.y / 100; 

    return (x * 8) + y; 
}