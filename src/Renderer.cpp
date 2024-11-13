#include "Renderer.h"

void Renderer::initialize(int height, int width) {
    m_x_offset = (static_cast<float>(height) - m_square_size * BOARD_SIZE) / 2.0f;
    m_y_offset = (static_cast<float>(width) - m_square_size * BOARD_SIZE) / 2.0f;
}

void Renderer::draw_board(const Board &board, sf::RenderWindow &window) {
    draw_empty_board(window);
    draw_pieces(board, window);
}


void Renderer::draw_dragged_piece(const Piece &piece, float x, float y, sf::RenderWindow &window) const {
    sf::Sprite sprite = m_sprite_manager->get_piece_sprite(piece);
    sprite.setPosition(x - m_square_size / 2, y - m_square_size / 2);
    window.draw(sprite);
}

// Returns the chess square row and column from a pixel position on the window
sf::Vector2i Renderer::get_square_location(const sf::Vector2i &position) const {
    const sf::Vector2i INVALID_SQUARE{-1, -1};

    if (!is_in_chess_board(position)) {
        return INVALID_SQUARE;
    }

    int row = static_cast<int>((position.y - m_y_offset) / m_square_size);
    int col = static_cast<int>((position.x - m_x_offset) / m_square_size);
    return {row, col};
}

bool Renderer::is_x_in_board(const float x) const {
    return (x <= ((1 + BOARD_SIZE) * m_square_size + m_x_offset) && x >= m_x_offset);
}

bool Renderer::is_y_in_board(const float y) const {
    return (y <= ((1 + BOARD_SIZE) * m_square_size + m_y_offset) && y >= m_y_offset);
}

bool Renderer::is_in_chess_board(sf::Vector2i position) const {
    return is_x_in_board(static_cast<float>(position.x)) && is_y_in_board(static_cast<float>(position.y));
}

sf::Color Renderer::get_square_color(const size_t row, const size_t col) const {
    return ((row + col) % 2)
               ? m_sprite_manager->get_light_square_color()
               : m_sprite_manager->get_dark_square_color();
}

sf::Vector2f Renderer::get_square_position(const size_t row, const size_t col) const {
    return {
        m_square_size * static_cast<float>(col) + m_x_offset,
        m_square_size * static_cast<float>(row) + m_y_offset
    };
}

void Renderer::draw_square(int row, int col, sf::RenderWindow &window) const {
    sf::RectangleShape rectangle;
    auto color = get_square_color(row, col);
    rectangle.setSize(sf::Vector2f(m_square_size, m_square_size));
    rectangle.setFillColor(color);
    rectangle.setOutlineThickness(0);
    rectangle.setPosition(get_square_position(row, col));
    window.draw(rectangle);
}

void Renderer::draw_empty_board(sf::RenderWindow &window) const {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            draw_square(row, col, window);
        }
    }
}

void Renderer::draw_piece(const Piece &piece, int row, int col, sf::RenderWindow &window) const {
    sf::Sprite sprite = m_sprite_manager->get_piece_sprite(piece);
    sprite.setPosition(get_square_position(row, col));
    window.draw(sprite);
}


void Renderer::draw_pieces(const Board &board, sf::RenderWindow &window) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (Piece piece = board.get_piece(row, col); piece.type != PieceType::NONE) {
                draw_piece(piece, row, col, window);
            }
        }
    }
}
