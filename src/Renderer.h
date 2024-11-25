//
// Created by pknadimp on 11/9/24.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include "Themes.h"
#include "Board.h"
#include "moves.h"
#include <vector>

class Renderer {
public:
    // Have this take in a theme ENUM? instead of a sprite manager
    Renderer(std::unique_ptr<SpriteManager> sprite_manager, unsigned int square_size) : m_square_size(
            static_cast<float>(square_size)),
        m_sprite_manager(std::move(sprite_manager)) {
    }

    void initialize(int height, int width);

    void draw_board(const Board &board, sf::RenderWindow &window);

    void draw_empty_square(int row, int col, sf::RenderWindow &window);

    [[nodiscard]] sf::Vector2i get_square_location(const sf::Vector2i &position) const;

    void draw_dragged_piece(const Piece &piece, float x, float y, sf::RenderWindow &window) const;

    sf::Color get_valid_square_color(const Position &position);

    void draw_available_moves(const std::vector<Move> &moves, sf::RenderWindow &window) const;

private:
    [[nodiscard]] bool is_x_in_board(float x) const;

    [[nodiscard]] bool is_y_in_board(float y) const;

    [[nodiscard]] bool is_in_chess_board(sf::Vector2i position) const;

    [[nodiscard]] sf::Color get_square_color(size_t row, size_t col) const;

    [[nodiscard]] sf::Vector2f get_square_position(size_t row, size_t col) const;

    [[nodiscard]] bool is_light_square(int row, int col) const;

    void draw_square(sf::Vector2f position, sf::RenderWindow &window, const sf::Color &color) const;

    void draw_chess_square(int row, int col, sf::RenderWindow &window) const;

    void draw_empty_board(sf::RenderWindow &window) const;

    void draw_piece(const Piece &piece, int row, int col, sf::RenderWindow &window) const;

    void draw_pieces(const Board &board, sf::RenderWindow &window);

    [[nodiscard]] sf::Color get_valid_square_color(const Position &position) const;

    void draw_valid_square(const Position &position, sf::RenderWindow &window) const;

private:
    std::unique_ptr<SpriteManager> m_sprite_manager;

    float m_square_size;
    float m_x_offset = 0;
    float m_y_offset = 0;
};


#endif //RENDERER_H
