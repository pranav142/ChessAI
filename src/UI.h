//
// Created by pknadimp on 11/19/24.
//

#ifndef UI_H
#define UI_H

#include "Board.h"
#include "Game.h"
#include "Renderer.h"

enum class UIStateType {
    IDLE,
    CLICKED,
    DRAGGED,
    DROPPED,
};

struct UIState {
    UIStateType type;
    Piece selected_piece;
    sf::Vector2i from_square;
    sf::Vector2i to_square;
    sf::Vector2i m_current_mouse_position;
};

void reset_state();

class UI {
public:
    UI();

    void run();

private:
    void handle_human_turn(const Player &player);

    void handle_computer_turn(const Player &player);

    void process_event(const sf::Event &event);

    void handle_user_input();

    void handle_window_close();

    void handle_mouse_button_press();

    void handle_mouse_button_release();

    void handle_mouse_move();

private:
    Renderer m_renderer;
    UIState m_state;
    Game m_game;

    sf::RenderWindow m_window;
    bool m_running = false;
};



#endif //UI_H
