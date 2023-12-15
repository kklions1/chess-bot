#pragma once 

#include <SFML/Graphics.hpp>
#include <unordered_map> 

#include "../common/common_structs.hpp"

sf::RectangleShape light_square(float, float);
sf::RectangleShape dark_square(float, float);
std::vector<sf::RectangleShape> init_squares();
std::unordered_map<int, std::shared_ptr<sf::Texture>> init_textures(); 