#pragma once 

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map> 

#include "../common/common_structs.hpp"
#include "piece_sprite.hpp" 

void gui_main(const Board&);
void handle_event_loop(const sf::Window&);
std::vector<sf::RectangleShape> init_squares();
std::unordered_map<int, std::shared_ptr<sf::Texture>> init_textures(); 
PieceSprite* get_piece_at_position(const sf::Vector2i&, std::vector<PieceSprite>&);
void snap_piece_to_square(const sf::Vector2f&, PieceSprite*);
void debug_print_vector(const sf::Vector2f&);