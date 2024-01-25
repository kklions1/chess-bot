#include "piece_sprite.hpp"

PieceSprite::PieceSprite(std::shared_ptr<sf::Texture> texture, std::shared_ptr<Piece> piece) { 
    this->piece = piece; 
    this->texture = texture; 
    this->shape = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
    this->shape.setTexture(texture.get());
}

// snaps the piece to the nearest board square to the position. usually the mouse position
void PieceSprite::setPosition(const sf::Vector2f& pos) { 
    this->shape.setPosition(normalize_to_corner(pos));
}

void PieceSprite::hide() { 
    this->shape.setFillColor(sf::Color(0));
}

std::vector<PieceSprite_ptr> init_sprites(Board& board, const TextureMap& texture_map) {
    std::vector<PieceSprite_ptr> result;
    for (int i = 0; i < 64; ++i) { 
        auto piece = board.board[i];
        if (piece->data != PieceType::EMPTY) { 
            auto sprite = std::make_shared<PieceSprite>(texture_map.at(piece->data), piece);
            sprite->shape.setPosition(calculate_position(i));
            result.push_back(sprite);
        }
    }

    return result;
}

// Calculates the position for a sprite based on its index on the board
sf::Vector2f calculate_position(int index) { 
    int file = index / 8;
    int rank = index % 8; 

    return sf::Vector2f(rank * 100, file * 100);
}

// Calculates the index in the board for a sprite based on its position 
int calculate_index(const sf::Vector2i& pos) { 
    int x = (pos.x / 100); 
    int y = (pos.y / 100); 

    return (y * 8) + x; 
}

int calculate_index(const sf::Vector2f& pos) { 
    // convert position into simple (x, y) coordinates 
    int x = (pos.x / 100); 
    int y = (pos.y / 100); 

    return (y * 8) + x; 
}

PieceSprite_ptr get_piece_at_position(const sf::Vector2i& pos, std::vector<PieceSprite_ptr>& sprites) {
    for (int i = 0; i < sprites.size(); ++i) { 
        if (sprites[i]->shape.getGlobalBounds().contains(sf::Vector2f(pos))) { 
            return sprites[i]; 
        }
    }

    return nullptr;
}

std::vector<PieceSprite_ptr>::iterator get_sprite_at_board_index(int index, std::vector<PieceSprite_ptr>& sprites) {
    sf::Vector2f pos = calculate_position(index);

    for (int i = 0; i < sprites.size(); ++i) { 
        if (sprites[i]->shape.getPosition() == sf::Vector2f(pos)) { 
            return sprites.begin() + i; 
        }
    }

    return sprites.end();
}

void snap_piece_to_square(const sf::Vector2f& mouse_pos, PieceSprite_ptr sprite) { 
    if (sprite == nullptr) return;

    sprite->setPosition(mouse_pos);
}

