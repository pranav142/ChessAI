//
// Created by pknadimp on 11/9/24.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "memory"
#include "Themes.h"
#include "Board.h"

class Renderer {
public:
    // Have this take in a theme ENUM? instead of a sprite manager
    Renderer(std::unique_ptr<SpriteManager> sprite_manager, unsigned int square_size) : m_square_size(
            static_cast<float>(square_size)),
        m_sprite_manager(std::move(sprite_manager)) {
    }

    void initialize(int height, int width);

    void draw_board(const Board &board, sf::RenderWindow &window);

    [[nodiscard]] sf::Vector2i get_square_location(const sf::Vector2i &position) const;

    void draw_dragged_piece(const Piece &piece, float x, float y, sf::RenderWindow &window) const;

private:
    [[nodiscard]] bool is_x_in_board(float x) const;

    [[nodiscard]] bool is_y_in_board(float y) const;

    [[nodiscard]] bool is_in_chess_board(sf::Vector2i position) const;

    [[nodiscard]] sf::Color get_square_color(size_t row, size_t col) const;

    [[nodiscard]] sf::Vector2f get_square_position(size_t row, size_t col) const;

    void draw_square(int row, int col, sf::RenderWindow &window) const;

    void draw_empty_board(sf::RenderWindow &window) const;

    void draw_piece(const Piece &piece, int row, int col, sf::RenderWindow &window) const;

    void draw_pieces(const Board &board, sf::RenderWindow &window);

private:
    std::unique_ptr<SpriteManager> m_sprite_manager;

    float m_square_size;
    float m_x_offset = 0;
    float m_y_offset = 0;
};


#endif //RENDERER_H
