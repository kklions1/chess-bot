#pragma once 

#include "../common/common_structs.hpp"
#include "gui_main.hpp"

#include <memory>

typedef std::unordered_map<int, std::shared_ptr<sf::Texture>> TextureMap; 

typedef struct PieceSprite { 
    std::shared_ptr<sf::Texture> texture;
    sf::RectangleShape shape;
    Piece_ptr piece;

    PieceSprite(std::shared_ptr<sf::Texture>, Piece_ptr); 
} PieceSprite; 

typedef std::shared_ptr<PieceSprite> PieceSprite_ptr; 

sf::Vector2f calculate_position(int);
int calculate_index(const sf::Vector2f&);
PieceSprite_ptr get_piece_at_position(const sf::Vector2i&, std::vector<PieceSprite_ptr>&);
void snap_piece_to_square(const sf::Vector2f&, PieceSprite_ptr);
std::vector<PieceSprite_ptr> init_sprites(Board&, const TextureMap&);
