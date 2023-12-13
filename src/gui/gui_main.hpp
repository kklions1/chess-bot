#pragma once 

#include <SFML/Window.hpp> 
#include <SFML/Graphics.hpp>

sf::RectangleShape light_square(float, float);
sf::RectangleShape dark_square(float, float);
std::vector<sf::RectangleShape> generate_squares();