#include "gui_main.hpp"

void draw_main_window() {
    sf::Window window(sf::VideoMode(800, 600), "Chess");
    
    while (window.isOpen()) { 
        sf::Event event;
        while (window.pollEvent(event)) { 
            if (event.type == sf::Event::Closed) { 
                window.close();
            }
            
        }
    }
}
