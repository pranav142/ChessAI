//
// Created by pknadimp on 11/30/24.
//

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Graphics.hpp>

enum class EventType {
    IDLE,
    CLICKED,
    DRAGGED,
    DROPPED,
    WINDOW_CLOSED,
};

struct Event {
    EventType type;
    sf::Vector2i original_mouse_position;
    sf::Vector2i current_mouse_position;
};

class InputManager {
public:
    InputManager();

    void update(sf::RenderWindow &window);

    [[nodiscard]] Event get_event() const;

private:
    void process_event(const sf::Event &event, sf::RenderWindow &window);

    void handle_window_close();

    void handle_mouse_button_press(const sf::RenderWindow &window);

    void handle_mouse_button_release(const sf::RenderWindow &window);

    void handle_mouse_move(const sf::RenderWindow &window);
private:
    Event m_current_event;
};

void reset_event(Event &event);
std::string event_type_to_string(EventType type);
void print_event(const Event &event);

#endif //INPUTMANAGER_H
