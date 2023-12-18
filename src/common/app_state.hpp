#pragma once 

#include <SFML/Graphics.hpp> 
#include <vector>
#include <unordered_map>
#include <memory>

#include "../gui/piece_sprite.hpp"

class AppState { 
private: 
    std::vector<sf::RectangleShape> board_squares;
    std::unordered_map<int, std::shared_ptr<sf::Texture>> texture_map;
    std::vector<PieceSprite> piece_sprites;
    bool is_dragging; 

    static AppState* instance; 

    AppState();

public: 
    AppState* getInstance();
};