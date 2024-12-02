//
// Created by pknadimp on 11/30/24.
//

#include "InputManager.h"

#include <iostream>

InputManager::InputManager() {
    reset_event(m_current_event);
}

void InputManager::update(sf::RenderWindow &window) {
    if (m_current_event.type == EventType::DROPPED) {
        reset_event(m_current_event);
        m_current_event.type = EventType::IDLE;
    }

    sf::Event event{};
    while (window.pollEvent(event)) {
        process_event(event, window);
    }
}

Event InputManager::get_event() const {
    return m_current_event;
}

void InputManager::process_event(const sf::Event &event, sf::RenderWindow &window) {
    switch (event.type) {
        case sf::Event::Closed: {
            handle_window_close();
            break;
        }
        case sf::Event::MouseButtonPressed: {
            handle_mouse_button_press(window);
            break;
        }
        case sf::Event::MouseMoved: {
            handle_mouse_move(window);
            break;
        }
        case sf::Event::MouseButtonReleased: {
            handle_mouse_button_release(window);
            break;
        }
        default: ;
    }
}


void InputManager::handle_mouse_button_press(const sf::RenderWindow &window) {
    m_current_event.type = EventType::CLICKED;
    m_current_event.current_mouse_position = sf::Mouse::getPosition(window);
    m_current_event.original_mouse_position = m_current_event.current_mouse_position;
}

void InputManager::handle_mouse_button_release(const sf::RenderWindow &window) {
    m_current_event.type = EventType::DROPPED;
    m_current_event.current_mouse_position = sf::Mouse::getPosition(window);
}

void InputManager::handle_mouse_move(const sf::RenderWindow &window) {
    if (m_current_event.type == EventType::IDLE || m_current_event.type == EventType::DROPPED) {
        return;
    }

    m_current_event.type = EventType::DRAGGED;
    m_current_event.current_mouse_position = sf::Mouse::getPosition(window);
}


void InputManager::handle_window_close() {
    reset_event(m_current_event);
    m_current_event.type = EventType::WINDOW_CLOSED;
}

std::string event_type_to_string(EventType type) {
    switch (type) {
        case EventType::IDLE:
            return "IDLE";
        case EventType::CLICKED:
            return "CLICKED";
        case EventType::DRAGGED:
            return "DRAGGED";
        case EventType::DROPPED:
            return "DROPPED";
        default:
            return "UNKNOWN";
    }
}

void print_event(const Event &event) {
    std::cout << "Event Type: " << event_type_to_string(event.type) << "\n"
            << "Original Mouse Position: (" << event.original_mouse_position.x << ", "
            << event.original_mouse_position.y << ")\n"
            << "Current Mouse Position: (" << event.current_mouse_position.x << ", "
            << event.current_mouse_position.y << ")\n";
}

void reset_event(Event &event) {
    event.type = EventType::IDLE;
    event.current_mouse_position = sf::Vector2i(-1, -1);
    event.original_mouse_position = event.current_mouse_position;
}
