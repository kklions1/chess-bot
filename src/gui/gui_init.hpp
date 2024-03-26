#pragma once 

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector> 

#include "../board/piece.hpp"

typedef std::unordered_map<int, std::shared_ptr<sf::Texture>> TextureMap; 

std::vector<sf::RectangleShape> init_squares();
TextureMap init_textures(); 