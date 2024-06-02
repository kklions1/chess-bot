#pragma once 

// #include "../board/board.hpp"
#include "../board/piece.hpp"
// #include "gui_main.hpp"

#include <memory>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>

typedef std::unordered_map<int, std::shared_ptr<sf::Texture>> TextureMap; 

class PieceSprite { 
public:
    std::shared_ptr<sf::Texture> texture;
    sf::RectangleShape shape;
    std::shared_ptr<Piece> piece;

    void setPosition(const sf::Vector2f&);
    void hide(); 

    static std::unique_ptr<PieceSprite> board_sprite(std::shared_ptr<sf::Texture>, std::shared_ptr<Piece>, sf::Vector2f); 
    static std::unique_ptr<PieceSprite> selection_sprite(std::shared_ptr<sf::Texture>, sf::Vector2f); 
private: 
    explicit PieceSprite(std::shared_ptr<sf::Texture>, std::shared_ptr<Piece>, sf::Vector2f); 
}; 

sf::Vector2f normalize_to_corner(const sf::Vector2f&); 

