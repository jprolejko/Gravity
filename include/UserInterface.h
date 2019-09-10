//
// Created by jprolejko on 27.02.19.
//

#ifndef GRAVITY_GRAPHICS_USERINTERFACE_H
#define GRAVITY_GRAPHICS_USERINTERFACE_H

#include <TGUI/TGUI.hpp>

#include <objects/AbstractDrawableObject.h>
#include <Config.h>


class UserInterface : public AbstractDrawableObject {

public:
    explicit UserInterface();

    UserInterface(const UserInterface&) = delete;
    UserInterface& operator=(const UserInterface&) = delete;

    void handleEvents();
    void drawGui();

private:
    enum class State {
        NOT_PRESSED,
        PRESSED
    };

    virtual void draw(sf::RenderTarget &target,
                      sf::RenderStates states) const;

    inline void addWidgets();

    inline void handleScrolling(sf::RenderWindow &graphics_window,
                                sf::View &view,
                                const sf::Vector2i &mouse_pos,
                                float zoom_delta);
    inline void handleKeyPressed(sf::RenderWindow &graphics_window,
                                 sf::View &view,
                                 const sf::Keyboard::Key &key_code);
    inline void handleInterfaceStates(sf::RenderWindow &graphics_window,
                                      const sf::Vector2i &mouse_pos,
                                      const sf::Vector2f &current_velocity);
    inline void setCursorRadius(float new_r);

    State state_;

    sf::Vector2i previous_mouse_pos_;

    sf::CircleShape cursor_planet_;
    float cursor_r_;
    float current_zoom_;

    // velocity graphics representation
    sf::ConvexShape shaft_;
    sf::ConvexShape arrow_l_, arrow_r_;

    tgui::Gui gui_;

};


#endif //GRAVITY_GRAPHICS_USERINTERFACE_H
