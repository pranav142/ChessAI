#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <utility>
#include "piece.h"

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

class SpriteManager {
public:
    virtual ~SpriteManager() = default;

    virtual sf::Sprite get_piece_sprite(const Piece &piece) = 0;

    virtual sf::Color get_dark_square_color() = 0;

    virtual sf::Color get_light_square_color() = 0;
};

class ClassicThemeManager final : public SpriteManager {
public:
    ClassicThemeManager() {
        m_empty_texture.create(1, 1);
        m_empty_sprite.setTexture(m_empty_texture);
    }

    sf::Sprite get_piece_sprite(const Piece &piece) override {
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

    sf::Color get_dark_square_color() override {
        return {118, 150, 86};
    }

    sf::Color get_light_square_color() override {
        return {238, 238, 210};
    }

private:
    static std::string get_piece_image_path(const Piece &piece) {
        const std::string color_name = piece.color == PieceColor::WHITE ? "white" : "black";
        return "../assets/" + get_piece_rule(piece.type)->piece_name + "/" + color_name + ".png";
    }

    static std::string get_key(const Piece &piece) {
        return get_piece_rule(piece.type)->piece_name + "_" + piece_color_to_string(piece.color);
    }

private:
    std::unordered_map<std::string, sf::Texture> m_texture_cache;
    sf::Sprite m_empty_sprite;
    sf::Texture m_empty_texture;
};

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

private:
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

void display_board(Piece (&board)[BOARD_SIZE][BOARD_SIZE]) {
    constexpr int height = 500;
    constexpr int width = static_cast<int>((9.0 / 9.0) * height);

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    auto theme = std::make_unique<ClassicThemeManager>();
    Renderer renderer(std::move(theme), 60);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        renderer.draw_board_to_window(board, window);
        window.display();
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
