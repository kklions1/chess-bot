#pragma once 

#include <SFML/Graphics.hpp> 
#include <memory> 

#include "gui_main.hpp"

class PromotionWindow { 
public: 
    sf::RectangleShape window;
    const TextureMap* textures; 

    static std::unique_ptr<PromotionWindow> init(const TextureMap*, int); 

private: 
    PromotionWindow(); 
    PromotionWindow(const TextureMap* textures) : textures(textures) {} 

}; 
