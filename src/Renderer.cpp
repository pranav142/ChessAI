#include "Renderer.h"

#include <iostream>
#include <unordered_set>

void Renderer::initialize(int height, int width) {
    m_x_offset = (static_cast<float>(height) - m_square_size * BOARD_SIZE) / 2.0f;
    m_y_offset = (static_cast<float>(width) - m_square_size * BOARD_SIZE) / 2.0f;
}

void Renderer::draw_board(const Board &board, sf::RenderWindow &window) {
    draw_empty_board(window);
    draw_pieces(board, window);
}

void Renderer::draw_empty_square(int row, int col, sf::RenderWindow &window) {
    auto color = get_square_color(row, col);
    draw_square(get_square_position(row, col), window, color);
}


void Renderer::draw_dragged_piece(const Piece &piece, float x, float y, sf::RenderWindow &window) const {
    sf::Sprite sprite = m_sprite_manager->get_piece_sprite(piece);
    sprite.setPosition(x - m_square_size / 2, y - m_square_size / 2);
    window.draw(sprite);
}

bool Renderer::is_light_square(int row, int col) const {
    return ((row + col) % 2);
}

sf::Color Renderer::get_valid_square_color(const Position &position) const {
    return is_light_square(position.row, position.col)
               ? m_sprite_manager->get_light_square_valid_color()
               : m_sprite_manager->get_dark_square_valid_color();
}

void Renderer::draw_valid_square(const Position &position, sf::RenderWindow &window) const {
    auto pixel_location = get_square_position(position.row, position.col);
    sf::Color color = get_valid_square_color(position);
    draw_square(pixel_location, window, color);
}

bool Renderer::square_is_in_vector(const Position &square, const std::vector<Position> &vector) const {
    for (const auto &pos: vector) {
        if (square.row == pos.row && square.col == pos.col) {
            return false;
        }
    }
    return true;
}

void Renderer::draw_available_moves(const std::vector<Move> &moves, sf::RenderWindow &window) const {
    std::vector<Position> drawn_squares;

    for (const auto &move: moves) {
        if (square_is_in_vector(move.to, drawn_squares)) {
            draw_valid_square(move.to, window);
            drawn_squares.push_back(move.to);
        }
    }
}

void Renderer::show_promotion_options(const PieceColor &piece_color, const Position &promotion_position,
                                      sf::RenderWindow &window) {
    m_showing_promotion_options = true;

    Piece pawn = {PieceType::PAWN, piece_color};

    int overlay_start_row = piece_color == PieceColor::WHITE ? promotion_position.row + 1 : promotion_position.row - 4;

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(m_square_size, m_square_size * 4));
    rectangle.setFillColor(m_sprite_manager->get_overlay_color());
    rectangle.setOutlineThickness(0);
    rectangle.setPosition(get_square_position(overlay_start_row, promotion_position.col));

    window.draw(rectangle);
    draw_piece(pawn, promotion_position.row, promotion_position.col, window);

    Piece pieces[4] = {
        {PieceType::BISHOP, piece_color},
        {PieceType::ROOK, piece_color},
        {PieceType::KNIGHT, piece_color},
        {PieceType::QUEEN, piece_color}
    };

    for (int i = 0; i < 4; i++) {
        draw_piece(pieces[i], overlay_start_row + i, promotion_position.col, window);
    }
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
    return (is_light_square(row, col))
               ? m_sprite_manager->get_light_square_color()
               : m_sprite_manager->get_dark_square_color();
}

sf::Vector2f Renderer::get_square_position(const size_t row, const size_t col) const {
    return {
        m_square_size * static_cast<float>(col) + m_x_offset,
        m_square_size * static_cast<float>(row) + m_y_offset
    };
}

void Renderer::draw_square(sf::Vector2f position, sf::RenderWindow &window, const sf::Color &color) const {
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(m_square_size, m_square_size));
    rectangle.setFillColor(color);
    rectangle.setOutlineThickness(0);
    rectangle.setPosition(position);
    window.draw(rectangle);
}

void Renderer::draw_chess_square(int row, int col, sf::RenderWindow &window) const {
    auto color = get_square_color(row, col);
    draw_square(get_square_position(row, col), window, color);
}

void Renderer::draw_empty_board(sf::RenderWindow &window) const {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            draw_chess_square(row, col, window);
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
