#include "gui_main.hpp"

#include <iostream>

sf::RectangleShape light_square(float x, float y) { 
    sf::RectangleShape square(sf::Vector2f(100.0f, 100.0f));
    square.setFillColor(sf::Color::White);
    square.setPosition(sf::Vector2f(x, y));
    
    // std::cout << "light square at (" << x << ", " << y << ")\n";

    return square;
}

sf::RectangleShape dark_square(float x, float y) { 
    sf::RectangleShape square(sf::Vector2f(100.0f, 100.0f));
    square.setFillColor(sf::Color::Black);
    square.setPosition(sf::Vector2f(x, y));
    
    // std::cout << "dark square at (" << x << ", " << y << ")\n";

    return square;
}

std::vector<sf::RectangleShape> generate_squares() { 
    float current_x = 0.0f;
    float current_y = 0.0f;
    std::vector<sf::RectangleShape> squares; 
    bool color_alternator = true;

    for (int rank = 0; rank < 8; rank++) { 
        for (int file = 0; file < 8; file++) { 
            if (color_alternator) { 
                squares.push_back(light_square(rank * 100, file * 100));
            } else { 
                squares.push_back(dark_square(rank * 100, file * 100));
            }

            color_alternator = !color_alternator;
        }
        color_alternator = !color_alternator;
    }

    return squares;
}