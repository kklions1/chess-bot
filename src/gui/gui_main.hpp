#pragma once 

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map> 
#include <memory>
#include <iostream>
#include <vector>

#include "../common/common_structs.hpp"
#include "piece_sprite.hpp" 

extern sf::RenderWindow main_window; 
typedef std::unordered_map<int, std::shared_ptr<sf::Texture>> TextureMap; 

void gui_main(const Board&);
void handle_event_loop();
std::vector<sf::RectangleShape> init_squares();
TextureMap init_textures(); 
void debug_print_vector(const sf::Vector2f&);
sf::Vector2f normalize_to_corner(const sf::Vector2f&); 

