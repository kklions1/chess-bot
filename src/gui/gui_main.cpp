#include "gui_main.hpp"

sf::RectangleShape light_square(float x, float y) { 
    sf::RectangleShape square(sf::Vector2f(100.0f, 100.0f));
    square.setFillColor(sf::Color::White);
    square.setPosition(sf::Vector2f(x, y));

    return square;
}

sf::RectangleShape dark_square(float x, float y) { 
    sf::RectangleShape square(sf::Vector2f(100.0f, 100.0f));
    square.setFillColor(sf::Color::Black);
    square.setPosition(sf::Vector2f(x, y));

    return square;
}
