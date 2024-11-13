//
// Created by pknadimp on 11/9/24.
//

#include "Game.h"
#include <iostream>

Game::Game()
    : m_renderer(std::make_unique<ClassicThemeManager>(), 60),
      m_window(sf::VideoMode(600, 600), "Chess Engine") {
}

void Game::initialize() {
    m_board.initialize();
    m_renderer.initialize(m_window.getSize().x, m_window.getSize().y);
}

void Game::handle_window_close() {
    m_running = false;
}

void Game::handle_mouse_button_press() {
    auto position_of_click = sf::Mouse::getPosition(m_window);
    auto square_index = m_renderer.get_square_location(position_of_click);
    if (!is_valid_square_index(square_index) || is_empty_square(square_index, m_board)) {
        return;
    }

    m_state = GameState::CLICKED;
    m_selected_piece = m_board.get_piece(square_index);
    m_current_position = position_of_click;
    m_original_square_position = square_index;
}

void Game::handle_mouse_button_release() {
    if (m_state != GameState::CLICKED) {
        return;
    }
    m_state = GameState::DROPPED;
    m_current_position = sf::Mouse::getPosition(m_window);
    m_current_square_position = m_renderer.get_square_location(m_current_position);
}

void Game::handle_mouse_move() {
    if (m_state != GameState::CLICKED) {
        return;
    }
    m_current_position = sf::Mouse::getPosition(m_window);
}

void Game::process_event(const sf::Event &event) {
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

void Game::update_state() {
    // Once we drop a piece we can reset the state machine
    if (m_state == GameState::DROPPED) {
        m_state = GameState::IDLE;
    }

    sf::Event event{};
    while (m_window.pollEvent(event)) {
        process_event(event);
    }
}

// TODO: Make Cleaner
void Game::update_board() {
    // If a piece is clicked we temporarily remove it from the board until it gets dropped
    if (m_state == GameState::CLICKED) {
        m_board.set_piece(m_original_square_position.x, m_original_square_position.y, create_piece(PieceType::NONE, PieceColor::NONE));
    }
    else if (m_state == GameState::DROPPED) {
        if (!is_valid_square_index(m_current_square_position)) {
            m_board.set_piece(m_original_square_position.x , m_original_square_position.y, m_selected_piece);
        } else {
            m_board.set_piece(m_current_square_position.x, m_current_square_position.y, m_selected_piece);
        }
    }
}

void Game::render() {
    m_window.clear();
    m_renderer.draw_board(m_board, m_window);
    if (m_state == GameState::CLICKED) {
        m_renderer.draw_dragged_piece(m_selected_piece, m_current_position.x, m_current_position.y, m_window);
    }
    m_window.display();
}

void Game::run() {
    m_running = true;

    while (m_running) {
        update_state();
        update_board();
        render();
    }
}
