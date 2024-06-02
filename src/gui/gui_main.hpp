#pragma once 

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map> 
#include <memory>
#include <iostream>
#include <vector>

#include "piece_sprite.hpp" 
#include "../board/board.hpp"

extern sf::RenderWindow main_window; 
typedef std::unordered_map<int, std::shared_ptr<sf::Texture>> TextureMap; 

sf::Vector2f calculate_position(int);
int calculate_index(const sf::Vector2i&);
int calculate_index(const sf::Vector2f&);
PieceSprite* get_piece_at_position(const sf::Vector2i&, const std::vector<std::unique_ptr<PieceSprite>>&);
int get_sprite_index_for_board_index(int, const std::vector<std::unique_ptr<PieceSprite>>&);
void snap_piece_to_square(const sf::Vector2f&, PieceSprite*);
std::vector<std::unique_ptr<PieceSprite>> init_sprites(Board*, const TextureMap&);
std::vector<sf::RectangleShape> init_squares();
TextureMap init_textures(); 
void debug_print_vector(const sf::Vector2f&);
void gui_main(std::shared_ptr<Board>);