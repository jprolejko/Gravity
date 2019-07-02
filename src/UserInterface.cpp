//
// Created by jprolejko on 27.02.19.
//

#include <iostream> 

#include <Engine.h>
#include <ResourceManager.h>
#include <utils/Geometry.h>
#include <graphics/Graphics.h>

#include <UserInterface.h>


UserInterface::UserInterface() {
    state_ = State::NOT_PRESSED;
    cursor_planet_.setTexture(&ResourceManager::getInstance().getTexture("planet"));
    cursor_planet_.setFillColor(sf::Color(Config::CURSOR_PLANET_COLOR_R_, Config::CURSOR_PLANET_COLOR_G_,
                                          Config::CURSOR_PLANET_COLOR_B_, Config::CURSOR_PLANET_COLOR_A_));
    setCursorRadius(10.0f);

    shaft_.setPointCount(3);
    arrow_l_.setPointCount(3);
    arrow_r_.setPointCount(3);

    sf::Color arrow_color(Config::ARROW_COLOR_R_, Config::ARROW_COLOR_G_,
                          Config::ARROW_COLOR_B_, Config::ARROW_COLOR_A_);
    shaft_.setFillColor(arrow_color);
    arrow_l_.setFillColor(arrow_color);
    arrow_r_.setFillColor(arrow_color);

    shaft_.setOrigin(0.0f, 0.0f);
    arrow_l_.setOrigin(0.0f, 0.0f);
    arrow_r_.setOrigin(0.0f, 0.0f);

    current_zoom_ = 1.0f;
}

void UserInterface::handleEvents() {
    static sf::Event event;
    static auto &graphics_window = Graphics::getInstance().getWindow();
    auto view = graphics_window.getView();
    auto mouse_pos = sf::Mouse::getPosition(graphics_window);
    auto mouse_difference = mouse_pos - previous_mouse_pos_;
    auto current_velocity = utils::vectorLengthLimit(static_cast<sf::Vector2f>(mouse_difference),
                                                     Config::MAX_SET_VELOCITY_ * Config::PIXELS_PER_KM_);

    while (graphics_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                graphics_window.close();
                break;
            }
            case sf::Event::Resized:
            {
                auto visible_area = sf::Vector2f(event.size.width, event.size.height);
                view.setSize(visible_area);

                graphics_window.setView(view);
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                previous_mouse_pos_ = mouse_pos;

                state_ = State::PRESSED;
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                Engine::getInstance().addPlanet(graphics_window.mapPixelToCoords(previous_mouse_pos_) /
                                                    Config::PIXELS_PER_KM_,
                                                current_velocity / Config::PIXELS_PER_KM_, cursor_r_);

                state_ = State::NOT_PRESSED;
                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                // TODO move to another method
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    setCursorRadius(cursor_r_ + cursor_r_ * event.mouseWheelScroll.delta / 4.0f);
                }
                else
                {
                    auto signum = event.mouseWheelScroll.delta < 0 ? -1.0f : 1.0f;
                    static constexpr float FACTOR = 0.1f;

                    auto zoom = 1.0f - event.mouseWheelScroll.delta * FACTOR;
                    
                    if (current_zoom_ * zoom <= Config::MAX_WINDOW_ZOOMOUT_ &&
                        current_zoom_ * zoom >= Config::MIN_WINDOW_ZOOMOUT_)
                    {
                        auto mouse_pos_from_center =
                            static_cast<sf::Vector2f>(sf::Mouse::getPosition(graphics_window)) -
                            static_cast<sf::Vector2f>(graphics_window.getSize()) / 2.0f;
                        view.setCenter(view.getCenter() + signum * mouse_pos_from_center * current_zoom_ * FACTOR);
                        current_zoom_ = current_zoom_ * zoom;
                        view.zoom(zoom);

                        graphics_window.setView(view);
                    }
                }

                break;
            }
            case sf::Event::KeyPressed:
            {
                // TODO move to another method
                auto delta = sf::Vector2f(0.0f, 0.0f);
                auto scrolling_speed = 30.0f * current_zoom_;

                switch (event.key.code)
                {
                    case sf::Keyboard::Left:
                    {
                        delta.x -= scrolling_speed;
                        break;
                    }
                    case sf::Keyboard::Right:
                    {
                        delta.x += scrolling_speed;
                        break;
                    }
                    case sf::Keyboard::Up:
                    {
                        delta.y -= scrolling_speed;
                        break;
                    }
                    case sf::Keyboard::Down:
                    {
                        delta.y += scrolling_speed;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                view.move(delta);
                graphics_window.setView(view);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    switch (state_)
    {
        case State::NOT_PRESSED:
        {
            cursor_planet_.setPosition(graphics_window.mapPixelToCoords(mouse_pos));
            previous_mouse_pos_ = mouse_pos;
            break;
        }
        case State::PRESSED:
        {
            // TODO Move to another method
            cursor_planet_.setPosition(graphics_window.mapPixelToCoords(previous_mouse_pos_));
            shaft_.setPosition(static_cast<sf::Vector2f>(previous_mouse_pos_));
            arrow_l_.setPosition(static_cast<sf::Vector2f>(previous_mouse_pos_));
            arrow_r_.setPosition(static_cast<sf::Vector2f>(previous_mouse_pos_));

            auto shaft_length = std::hypot(current_velocity.x, current_velocity.y) - Config::ARROW_WIDTH_ * 3.0f;
            auto arrow_rotation = 
                static_cast<float>(std::atan2(current_velocity.y, current_velocity.x) / M_PI * 180.0f);

            shaft_length = utils::isNearlyEqual(shaft_length, 
                - Config::ARROW_WIDTH_ * 3.0f, Config::ARROW_WIDTH_ * 6.0f) ? 0.0f : shaft_length;

            shaft_.setPoint(0, {0, 0});
            shaft_.setPoint(1, {shaft_length, - Config::ARROW_WIDTH_ / 2.0f});
            shaft_.setPoint(2, {shaft_length, Config::ARROW_WIDTH_ / 2.0f});
            
            arrow_l_.setPoint(0, {shaft_length, 0});
            arrow_l_.setPoint(1, {shaft_length + Config::ARROW_WIDTH_ * 3.0f, 0});
            arrow_l_.setPoint(2, {shaft_length - Config::ARROW_WIDTH_ * 3.0f, - Config::ARROW_WIDTH_ * 3.0f});

            arrow_r_.setPoint(0, {shaft_length, 0});
            arrow_r_.setPoint(1, {shaft_length + Config::ARROW_WIDTH_ * 3.0f, 0});
            arrow_r_.setPoint(2, {shaft_length - Config::ARROW_WIDTH_ * 3.0f, Config::ARROW_WIDTH_ * 3.0f});

            shaft_.setRotation(arrow_rotation);
            arrow_l_.setRotation(arrow_rotation);
            arrow_r_.setRotation(arrow_rotation);

            break;
        }
        default:
        {
            throw std::runtime_error("[UserInterface] Wrong state!");
            break;
        }
    }
}

void UserInterface::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (state_ == State::PRESSED && !utils::isNearlyEqual(shaft_.getLocalBounds().width, 0.0f, 0.01f))
    {
        target.draw(shaft_, states);
        target.draw(arrow_l_, states);
        target.draw(arrow_r_, states);
    }

    Graphics::getInstance().setDynamicView();
    target.draw(cursor_planet_, states);
    Graphics::getInstance().setStaticView();
}

inline void UserInterface::setCursorRadius(float new_r) {
    if (new_r <= 0.0 || new_r >= Config::MAX_PLANET_RADIUS_)
    {
        return;
    }

    cursor_r_ = new_r;

    cursor_planet_.setOrigin(cursor_r_ * Config::PIXELS_PER_KM_, cursor_r_ * Config::PIXELS_PER_KM_);
    cursor_planet_.setRadius(cursor_r_ * Config::PIXELS_PER_KM_);
}
