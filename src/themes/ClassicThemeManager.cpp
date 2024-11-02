#include "Themes.h"

#include <iostream>

ClassicThemeManager::ClassicThemeManager() {
    m_empty_texture.create(1, 1);
    m_empty_sprite.setTexture(m_empty_texture);
}

sf::Sprite ClassicThemeManager::get_piece_sprite(const Piece &piece) {
    if (piece.type == PieceType::NONE) {
        return m_empty_sprite;
    }

    sf::Sprite sprite;
    sf::Texture texture;
    std::string key = get_key(piece);

    if (m_texture_cache.find(key) != m_texture_cache.end()) {
        sprite.setTexture(m_texture_cache[key]);
        return sprite;
    }

    if (!texture.loadFromFile(get_piece_image_path(piece))) {
        std::cerr << "Failed to load piece image" << std::endl;
        return m_empty_sprite;
    }

    m_texture_cache[key] = texture;
    sprite.setTexture(texture);
    return sprite;
}

sf::Color ClassicThemeManager::get_dark_square_color() {
    return {118, 150, 86};
}

sf::Color ClassicThemeManager::get_light_square_color() {
    return {238, 238, 210};
}

std::string ClassicThemeManager::get_piece_image_path(const Piece &piece) {
    const std::string color_name = piece.color == PieceColor::WHITE ? "white" : "black";
    return "../assets/" + get_piece_rule(piece.type)->piece_name + "/" + color_name + ".png";
}

std::string ClassicThemeManager::get_key(const Piece &piece) {
    return get_piece_rule(piece.type)->piece_name + "_" + piece_color_to_string(piece.color);
}
