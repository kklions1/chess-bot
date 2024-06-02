#include "piece_sprite.hpp"

PieceSprite::PieceSprite(std::shared_ptr<sf::Texture> texture, std::shared_ptr<Piece> piece, sf::Vector2f position) { 
    this->piece = piece; 
    this->texture = texture; 
    this->shape = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
    this->shape.setTexture(texture.get());
    this->shape.setPosition(position); 
}

// snaps the piece to the nearest board square to the position. usually the mouse position
void PieceSprite::setPosition(const sf::Vector2f& pos) { 
    this->shape.setPosition(normalize_to_corner(pos));
}

void PieceSprite::hide() { 
    this->shape.setFillColor(sf::Color(0));
}

std::unique_ptr<PieceSprite> PieceSprite::board_sprite(std::shared_ptr<sf::Texture> texture, std::shared_ptr<Piece> piece, sf::Vector2f pos) { 
    return std::unique_ptr<PieceSprite>(new PieceSprite(texture, piece, pos));
}

std::unique_ptr<PieceSprite> PieceSprite::selection_sprite(std::shared_ptr<sf::Texture> texture, sf::Vector2f pos) { 
    return std::unique_ptr<PieceSprite>(new PieceSprite(texture, nullptr, pos));
}

sf::Vector2f normalize_to_corner(const sf::Vector2f& pos) {
    return sf::Vector2f((((int) pos.x / 100) * 100), ((int) (pos.y / 100) * 100));
}