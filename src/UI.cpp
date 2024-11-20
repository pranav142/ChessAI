//
// Created by pknadimp on 11/19/24.
//

#include "UI.h"

#include <iostream>


UI::UI(): m_renderer(std::make_unique<ClassicThemeManager>(), 60), m_window(sf::VideoMode(480, 480), "Chess Engine"),
          m_state(UIState{UIStateType::IDLE}) {
    m_game.initialize();
    m_renderer.initialize(m_window.getSize().x, m_window.getSize().y);
}

void UI::handle_window_close() {
    m_running = false;
}

void UI::handle_mouse_button_press() {
    auto position_of_click = sf::Mouse::getPosition(m_window);
    auto square_index = m_renderer.get_square_location(position_of_click);
    if (!is_valid_square_index(square_index) || m_game.is_empty_square(square_index.x, square_index.y)) {
        return;
    }

    m_state.type = UIStateType::CLICKED;
    m_state.selected_piece = m_game.get_piece(square_index.x, square_index.y);
    m_state.m_current_mouse_position = position_of_click;
    m_state.from_square = square_index;
}

void UI::handle_mouse_button_release() {
    if (m_state.type != UIStateType::CLICKED) {
        return;
    }

    m_state.type = UIStateType::DROPPED;
    m_state.m_current_mouse_position = sf::Mouse::getPosition(m_window);
    m_state.to_square = m_renderer.get_square_location(m_state.m_current_mouse_position);
}

void UI::handle_mouse_move() {
    if (m_state.type != UIStateType::CLICKED) {
        return;
    }

    m_state.m_current_mouse_position = sf::Mouse::getPosition(m_window);
}

void UI::process_event(const sf::Event &event) {
    switch (event.type) {
        case sf::Event::Closed: {
            handle_window_close();
            break;
        }
        case sf::Event::MouseButtonPressed: {
            handle_mouse_button_press();
            break;
        }
        case sf::Event::MouseMoved: {
            handle_mouse_move();
            break;
        }
        case sf::Event::MouseButtonReleased: {
            handle_mouse_button_release();
            break;
        }
        default: ;
    }
}

void UI::handle_human_turn(const Player &player) {
    // if (m_state.type == UIStateType::DROPPED) {
    //     // to be implemented
    //     reset_state();
    // }
    // Process human interactions
    // if a piece gets dropped
    switch (m_state.type) {
        case UIStateType::CLICKED: {
            if (player.color != m_state.selected_piece.color) {
                m_state.type = UIStateType::IDLE;
                break;
            }
            auto moves = m_game.get_moves(m_state.selected_piece, m_state.from_square.x, m_state.from_square.y);
            m_renderer.draw_empty_square(m_state.from_square.x, m_state.from_square.y, m_window);
            m_renderer.draw_available_moves(moves, m_window);
            m_renderer.draw_dragged_piece(m_state.selected_piece, m_state.m_current_mouse_position.x,
                                          m_state.m_current_mouse_position.y, m_window);
            break;
        }
        case UIStateType::DROPPED: {
            if (m_game.is_move_valid(m_state.selected_piece, m_state.from_square.x, m_state.from_square.y,
                                     m_state.to_square.x, m_state.to_square.y)) {
                m_game.make_move(m_state.selected_piece, m_state.from_square.x, m_state.from_square.y,
                                 m_state.to_square.x, m_state.to_square.y);
                m_renderer.draw_board(m_game.get_board(), m_window);
            }
            m_state.type = UIStateType::IDLE;
            break;
        }
        default: {
        }
    }
}

void UI::handle_computer_turn(const Player &player) {
    m_game.make_computer_move(player.color);
}

void UI::handle_user_input() {
    sf::Event event{};
    while (m_window.pollEvent(event)) {
        process_event(event);
    }
}

void UI::run() {
    m_running = true;

    while (m_running) {
        auto board = m_game.get_board();
        m_renderer.draw_board(board, m_window);
        handle_user_input();

        auto current_player = m_game.get_current_player();
        if (current_player.type == PlayerType::HUMAN) {
            handle_human_turn(current_player);
        }
        if (current_player.type == PlayerType::AI) {
            handle_computer_turn(current_player);
        }

        m_window.display();
    }
}
