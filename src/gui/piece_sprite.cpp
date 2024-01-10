#include "piece_sprite.hpp"

PieceSprite::PieceSprite(std::shared_ptr<sf::Texture> texture, const Piece* piece) { 
    this->piece = piece; 
    this->texture = texture; 
    this->shape = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
    this->shape.setTexture(texture.get());
}

std::vector<PieceSprite> init_sprites(const Board& board, const TextureMap& texture_map) {
    std::vector<PieceSprite> result;
    for (int i = 0; i < 64; ++i) { 
        const Piece* piece = &board.board[i];
        if (piece->data != PieceType::EMPTY) { 
            PieceSprite sprite = PieceSprite(texture_map.at(piece->data), piece);
            sprite.shape.setPosition(calculate_position(i));
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