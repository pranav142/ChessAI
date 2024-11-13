#ifndef THEMES_H
#define THEMES_H

#include <SFML/Graphics.hpp>
#include "Piece.h"

class SpriteManager {
public:
    virtual ~SpriteManager() = default;

    virtual sf::Sprite get_piece_sprite(const Piece &piece) = 0;

    virtual sf::Color get_dark_square_color() = 0;

    virtual sf::Color get_light_square_color() = 0;
};

class ClassicThemeManager final : public SpriteManager {
public:
    ClassicThemeManager();

    sf::Sprite get_piece_sprite(const Piece &piece) override;

    sf::Color get_dark_square_color() override;

    sf::Color get_light_square_color() override;

private:
    static std::string get_piece_image_path(const Piece &piece);

    static std::string get_key(const Piece &piece);

private:
    std::unordered_map<std::string, sf::Texture> m_texture_cache;
    sf::Sprite m_empty_sprite;
    sf::Texture m_empty_texture;
};


#endif //THEMES_H
