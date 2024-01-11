#pragma once 

#include "../common/common_structs.hpp"
#include "gui_main.hpp"

typedef std::unordered_map<int, std::shared_ptr<sf::Texture>> TextureMap; 

typedef struct PieceSprite { 
    std::shared_ptr<sf::Texture> texture;
    sf::RectangleShape shape;
    std::shared_ptr<Piece> piece;

    PieceSprite(std::shared_ptr<sf::Texture>, std::shared_ptr<Piece>); 
} PieceSprite; 

sf::Vector2f calculate_position(int);
int calculate_index(const sf::Vector2f&);
PieceSprite* get_piece_at_position(const sf::Vector2i&, std::vector<PieceSprite>&);
void snap_piece_to_square(const sf::Vector2f&, PieceSprite*);
std::vector<PieceSprite> init_sprites(Board&, const TextureMap&);
