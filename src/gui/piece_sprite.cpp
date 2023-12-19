#include "piece_sprite.hpp"

PieceSprite create_sprite(std::shared_ptr<sf::Texture> texture) { 
    auto sprite = PieceSprite(); 
    sprite.texture = texture;
    sprite.shape = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
    sprite.shape.setTexture(sprite.texture.get());
    
    return sprite;
}

sf::Vector2f calculate_position(int index) { 
    int file = index / 8;
    int rank = index % 8; 

    return sf::Vector2f(rank * 100, file * 100);
}

int calculate_index(const sf::Vector2f& pos) { 
    int x = pos.x / 100; 
    int y = pos.y / 100; 

    return (x * 8) + y; 
}

PieceSprite* get_piece_at_position(const sf::Vector2i& click_pos, std::vector<PieceSprite>& sprites) {
    for (int i = 0; i < sprites.size(); ++i) { 
        if (sprites[i].shape.getGlobalBounds().contains(sf::Vector2f(click_pos))) { 
            return &sprites[i]; 
        }
    }

    return nullptr;
}

void snap_piece_to_square(const sf::Vector2f& mouse_pos, PieceSprite* sprite) { 
    if (sprite != nullptr) { 
        sprite->shape.setPosition(sf::Vector2f(normalize_to_corner(mouse_pos)));
    }
}
