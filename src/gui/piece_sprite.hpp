#pragma once 

#include "../board/board.hpp"
#include "../board/piece.hpp"
#include "gui_main.hpp"

#include <memory>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>

typedef std::unordered_map<int, std::shared_ptr<sf::Texture>> TextureMap; 

typedef struct PieceSprite { 
    std::shared_ptr<sf::Texture> texture;
    sf::RectangleShape shape;
    std::shared_ptr<Piece> piece;

    PieceSprite(std::shared_ptr<sf::Texture>, std::shared_ptr<Piece>); 

    void setPosition(const sf::Vector2f&);
    void hide(); 

    static std::unique_ptr<PieceSprite> init(); 
} PieceSprite; 

typedef std::shared_ptr<PieceSprite> PieceSprite_ptr; 

sf::Vector2f calculate_position(int);
int calculate_index(const sf::Vector2i&);
int calculate_index(const sf::Vector2f&);
PieceSprite_ptr get_piece_at_position(const sf::Vector2i&, std::vector<PieceSprite_ptr>&);
std::vector<PieceSprite_ptr>::iterator get_sprite_at_board_index(int, std::vector<PieceSprite_ptr>&);
void snap_piece_to_square(const sf::Vector2f&, PieceSprite_ptr);
std::vector<PieceSprite_ptr> init_sprites(Board*, const TextureMap&);
