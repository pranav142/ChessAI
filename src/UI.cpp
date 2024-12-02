//
// Created by pknadimp on 11/19/24.
//

#include "UI.h"

#include <thread>

UI::UI(): m_renderer(std::make_unique<ClassicThemeManager>(), 60), m_window(sf::VideoMode(480, 480), "Chess Engine"),
          m_screen_state(ScreenStateType::PLAYING) {
    m_game.initialize();
    m_renderer.initialize(m_window.getSize().x, m_window.getSize().y);
}

Piece UI::get_piece(const sf::Vector2i &position) const {
    sf::Vector2i square = m_renderer.get_square_location(position);
    if (square.x == -1 || square.y == -1) {
        return {PieceType::NONE, PieceColor::NONE};
    }
    return m_game.get_piece(square.x, square.y);
}

void UI::handle_piece_clicked(const Player &player, const Event &event) {
    Piece selected_piece = get_piece(event.original_mouse_position);
    if (player.color != selected_piece.color || selected_piece.type == PieceType::NONE || selected_piece.color ==
        PieceColor::NONE) {
        return;
    }

    sf::Vector2i selected_square = m_renderer.get_square_location(event.original_mouse_position);
    auto moves = m_game.get_moves(selected_piece, selected_square.x, selected_square.y);
    m_renderer.draw_empty_square(selected_square.x, selected_square.y, m_window);
    m_renderer.draw_available_moves(moves, m_window);
    m_renderer.draw_dragged_piece(selected_piece, event.current_mouse_position.x,
                                  event.current_mouse_position.y, m_window);
}

// Cleaner state management needed
void UI::handle_piece_dropped(const Player &player, const Event &event) {
    Piece selected_piece = get_piece(event.original_mouse_position);
    if (player.color != selected_piece.color || selected_piece.type == PieceType::NONE || selected_piece.color ==
        PieceColor::NONE) {
        return;
    }

    sf::Vector2i from_square = m_renderer.get_square_location(event.original_mouse_position);
    sf::Vector2i to_square = m_renderer.get_square_location(event.current_mouse_position);

    if (m_game.is_move_valid(selected_piece, from_square.x, from_square.y,
                             to_square.x, to_square.y)) {
        if (is_promotion_move(selected_piece, to_square.x)) {
            m_promotion_state.from_square = from_square;
            m_promotion_state.to_square = to_square;
            m_promotion_state.selected_piece = selected_piece;
            m_screen_state = ScreenStateType::PROMOTION;
        } else {
            m_game.make_move(selected_piece, from_square.x, from_square.y,
                             to_square.x, to_square.y, {PieceType::NONE, PieceColor::NONE});
            m_renderer.draw_board(m_game.get_board(), m_window);
        }
    }
}

void UI::handle_human_turn(const Player &player, const Event &event) {
    switch (event.type) {
        case EventType::CLICKED: {
            handle_piece_clicked(player, event);
            break;
        }
        case EventType::DROPPED: {
            handle_piece_dropped(player, event);
            break;
        }
        case EventType::DRAGGED: {
            handle_piece_clicked(player, event);
            break;
        }
        default: {
        }
    }
}

void UI::handle_computer_turn(const Player &player) {
    m_game.make_computer_move(player.color);
}

void UI::handle_user_input(const Event &event) {
    if (event.type == EventType::WINDOW_CLOSED) {
        m_running = false;
        return;
    }

    m_renderer.draw_board(m_game.get_board(), m_window);

    if (m_screen_state == ScreenStateType::PLAYING) {
        auto current_player = m_game.get_current_player();
        if (current_player.type == PlayerType::HUMAN) {
            handle_human_turn(current_player, event);
        }
        if (current_player.type == PlayerType::AI) {
            handle_computer_turn(current_player);
        }
    }

    if (m_screen_state == ScreenStateType::PROMOTION) {
        handle_promotion_rendering();
        handle_promotion_piece_selection(event);
    }
}

void UI::handle_promotion_rendering() {
    m_renderer.draw_empty_square(m_promotion_state.from_square.x, m_promotion_state.from_square.y, m_window);
    m_renderer.draw_empty_square(m_promotion_state.to_square.x, m_promotion_state.to_square.y, m_window);
    m_renderer.show_promotion_options(m_promotion_state.selected_piece.color,
                                      Position{m_promotion_state.to_square.x, m_promotion_state.to_square.y}, m_window);
}


void UI::handle_promotion_piece_selection(const Event &event) {
    if (event.type == EventType::CLICKED) {
        Piece piece = m_renderer.get_promotion_option(event.current_mouse_position,
                                                      Position{
                                                          m_promotion_state.to_square.x, m_promotion_state.to_square.y
                                                      }, m_promotion_state.selected_piece.color);
        //print_piece(piece);
       if (piece.type == PieceType::NONE) {
            return;
       }
        m_renderer.draw_empty_square(m_promotion_state.to_square.x, m_promotion_state.to_square.y, m_window);
        m_game.make_move(m_promotion_state.selected_piece, m_promotion_state.from_square.x, m_promotion_state.from_square.y, m_promotion_state.to_square.x, m_promotion_state.to_square.y, piece);
        m_screen_state = ScreenStateType::PLAYING;
    }
}

Event UI::get_user_input() {
    m_input_manager.update(m_window);
    return m_input_manager.get_event();
}

void UI::run() {
    m_running = true;

    while (m_running) {
        m_window.clear();
        Event event = get_user_input();
        handle_user_input(event);
        m_window.display();
    }
}
