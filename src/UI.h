//
// Created by pknadimp on 11/19/24.
//

#ifndef UI_H
#define UI_H

#include "Game.h"
#include "InputManager.h"
#include "Renderer.h"
#include "move_generation.h"

enum class ScreenStateType {
    PLAYING,
    PROMOTION,
    GAME_OVER,
};

struct PromotionState {
    sf::Vector2i from_square;
    sf::Vector2i to_square;
    Piece selected_piece;
};

void reset_state();

class UI {
public:
    UI();

    void run();

private:
    void handle_human_turn(const Player &player, const Event &event);

    void handle_computer_turn(const Player &player);

    void handle_end_screen_rendering();

    void process_event(const sf::Event &event);

    Piece get_piece(const sf::Vector2i &position) const;

    void handle_user_input(const Event &event);

    void handle_promotion_rendering();

    void handle_promotion_piece_selection(const Event &event);

    Event get_user_input();

    void handle_piece_clicked(const Player &player, const Event &event);

    void handle_piece_dropped(const Player &player, const Event &event);

private:
    Renderer m_renderer;
    InputManager m_input_manager;
    Game m_game;
    ScreenStateType m_screen_state;
    PromotionState m_promotion_state;

    sf::RenderWindow m_window;
    bool m_running = false;
};



#endif //UI_H
