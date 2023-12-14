#pragma once 

#include <SFML/Window.hpp> 
#include <SFML/Graphics.hpp>
#include <unordered_map> 

#include "piece_sprite.hpp"

sf::RectangleShape light_square(float, float);
sf::RectangleShape dark_square(float, float);
std::vector<sf::RectangleShape> generate_squares();
std::unordered_map<int, std::shared_ptr<sf::Texture>> init_textures(); 