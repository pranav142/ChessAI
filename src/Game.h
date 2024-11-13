#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Renderer.h"

enum class GameState {
    IDLE,
    CLICKED,
    DROPPED,
};

class Game {
public:
    Game();

    ~Game() = default;

    void run();

    void initialize();

private:
    void update_state();

    void update_board();

    void render();

    void process_event(const sf::Event &event);

    void handle_window_close();

    void handle_mouse_button_press();

    void handle_mouse_button_release();

    void handle_mouse_move();

private:
    Board m_board;
    Renderer m_renderer;
    sf::RenderWindow m_window;

    bool m_running = false;

    // TODO: Evaluate how to clean up game state management
    GameState m_state = GameState::IDLE;
    sf::Vector2i m_original_square_position;
    sf::Vector2i m_current_position;
    sf::Vector2i m_current_square_position;
    Piece m_selected_piece;
};

#endif //GAME_H
