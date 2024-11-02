#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <utility>
#include <thread>
#include <chrono>
#include "Piece.h"
#include "Themes.h"

constexpr size_t BOARD_SIZE = 8;

void initialize_board(Piece (&board)[BOARD_SIZE][BOARD_SIZE]) {
    constexpr PieceType back_row[BOARD_SIZE] = {
        PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
        PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
    };

    for (auto &row: board) {
        for (auto &square: row) {
            square = create_piece(PieceType::NONE, PieceColor::NONE);
        }
    }

    for (int col = 0; col < BOARD_SIZE; col++) {
        board[6][col] = create_piece(PieceType::PAWN, PieceColor::WHITE);
        board[1][col] = create_piece(PieceType::PAWN, PieceColor::BLACK);

        board[7][col] = create_piece(back_row[col], PieceColor::WHITE);
        board[0][col] = create_piece(back_row[col], PieceColor::BLACK);
    }
}

void print_board(Piece (&board)[BOARD_SIZE][BOARD_SIZE]) {
    for (auto &row: board) {
        std::cout << '\n';
        for (const auto [type, color]: row) {
            char color_char = '0';
            if (color == PieceColor::WHITE) {
                color_char = 'w';
            } else if (color == PieceColor::BLACK) {
                color_char = 'b';
            }

            std::cout << color_char << get_piece_rule(type)->letter << ' ';
        }
    }
    std::cout << std::endl;
}

class Renderer {
public:
    Renderer(std::unique_ptr<SpriteManager> sprite_manager, unsigned int square_size) : m_square_size(
            static_cast<float>(square_size)),
        m_sprite_manager(std::move(sprite_manager)) {
    }

    void draw_board_to_window(const Piece (&board)[BOARD_SIZE][BOARD_SIZE], sf::RenderWindow &window) {
        initialize(window);

        draw_empty_board(window);
        draw_pieces(window, board);
    }

    [[nodiscard]] sf::Vector2i get_square_location(const sf::Vector2i &position) const {
        const sf::Vector2i INVALID_SQUARE{-1, -1};

        if (!is_in_chess_board(position)) {
            return INVALID_SQUARE;
        }

        int row = static_cast<int>((position.y - m_y_offset) / m_square_size);
        int col = static_cast<int>((position.x - m_x_offset) / m_square_size);
        return {row, col};
    }

private:
    [[nodiscard]] bool is_x_in_board(const float x) const {
        return (x <= ((1 + BOARD_SIZE) * m_square_size + m_x_offset) && x >= m_x_offset);
    }

    [[nodiscard]] bool is_y_in_board(const float y) const {
        return (y <= ((1 + BOARD_SIZE) * m_square_size + m_y_offset) && y >= m_y_offset);
    }

    [[nodiscard]] bool is_in_chess_board(sf::Vector2i position) const {
        return is_x_in_board(static_cast<float>(position.x)) && is_y_in_board(static_cast<float>(position.y));
    }

    void initialize(const sf::RenderWindow &window) {
        m_size = sf::Vector2f(window.getSize());

        m_x_offset = (m_size.x - m_square_size * BOARD_SIZE) / 2.0f;
        m_y_offset = (m_size.y - m_square_size * BOARD_SIZE) / 2.0f;
    }

    [[nodiscard]] sf::Color get_square_color(const size_t row, const size_t col) const {
        return ((row + col) % 2)
                   ? m_sprite_manager->get_light_square_color()
                   : m_sprite_manager->get_dark_square_color();
    }

    [[nodiscard]] sf::Vector2f get_square_position(const size_t row, const size_t col) const {
        return {
            m_square_size * static_cast<float>(col) + m_x_offset,
            m_square_size * static_cast<float>(row) + m_y_offset
        };
    }

    void draw_empty_board(sf::RenderWindow &window) const {
        for (size_t row = 0; row < BOARD_SIZE; row++) {
            for (size_t col = 0; col < BOARD_SIZE; col++) {
                sf::RectangleShape rectangle;
                auto color = get_square_color(row, col);
                rectangle.setSize(sf::Vector2f(m_square_size, m_square_size));
                rectangle.setFillColor(color);
                rectangle.setOutlineThickness(0);
                rectangle.setPosition(get_square_position(row, col));
                window.draw(rectangle);
            }
        }
    }

    void draw_pieces(sf::RenderWindow &window, const Piece (&board)[BOARD_SIZE][BOARD_SIZE]) const {
        for (size_t row = 0; row < BOARD_SIZE; row++) {
            for (size_t col = 0; col < BOARD_SIZE; col++) {
                if (Piece piece = board[row][col]; piece.type != PieceType::NONE) {
                    sf::Sprite sprite = m_sprite_manager->get_piece_sprite(piece);
                    sprite.setPosition(get_square_position(row, col));
                    window.draw(sprite);
                }
            }
        }
    }

private:
    float m_square_size;
    std::unique_ptr<SpriteManager> m_sprite_manager;

    float m_x_offset = 0;
    float m_y_offset = 0;
    sf::Vector2f m_size = {0, 0};
};

enum class press_state {
    INITIAL,
    DRAG,
};

void display_board(Piece (&board)[BOARD_SIZE][BOARD_SIZE]) {
    constexpr int height = 500;
    constexpr int width = static_cast<int>((9.0 / 9.0) * height);

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    auto theme = std::make_unique<ClassicThemeManager>();
    Renderer renderer(std::move(theme), 60);
    press_state current_state = press_state::INITIAL;

    Piece clicked_piece;
    sf::Vector2i original_location;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        window.clear();
        renderer.draw_board_to_window(board, window);

        bool is_button_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        std::clog << static_cast<int>(current_state) << '\n';
        switch (current_state) {
            case press_state::INITIAL: {
                if (is_button_pressed) {
                    auto position_of_click = sf::Mouse::getPosition(window);
                    auto square_location = renderer.get_square_location(position_of_click);
                    if (square_location.x < BOARD_SIZE && square_location.y < BOARD_SIZE && square_location.x >= 0 &&
                        square_location.y >= 0) {
                        std::clog << get_piece_rule(board[square_location.x][square_location.y].type)->piece_name <<
                                '\n';
                        clicked_piece = board[square_location.x][square_location.y];
                        print_piece(clicked_piece);
                        original_location = square_location;
                        current_state = press_state::DRAG;
                    }
                }
                break;
            }
            case press_state::DRAG: {
                print_piece(clicked_piece);
                if (!is_button_pressed) {
                    // clicked_piece = {PieceType::NONE, PieceColor::NONE};
                    auto position_of_click = sf::Mouse::getPosition(window);
                    auto square_location = renderer.get_square_location(position_of_click);
                    std::clog << square_location.x << " " << square_location.y << '\n';
                    if (square_location.x < BOARD_SIZE && square_location.y < BOARD_SIZE && square_location.x >= 0 &&
                        square_location.y >= 0) {
                        board[square_location.x][square_location.y] = clicked_piece;
                        board[original_location.x][original_location.y] = Piece{PieceType::NONE, PieceColor::NONE};
                    }
                    current_state = press_state::INITIAL;
                }

                break;
            }
            // set clicked piece position to the position of the mouse
        }


        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(32));
    }
}

// TODO: RUN APP
// Main event loop, Captures movements, Some object to manage game state, some way to display game;

int main() {
    Piece board[BOARD_SIZE][BOARD_SIZE];
    initialize_board(board);
    print_board(board);
    display_board(board);
    return 0;
}
