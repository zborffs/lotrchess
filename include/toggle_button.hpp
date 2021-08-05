#ifndef LOTRCHESS_TOGGLE_BUTTON_HPP
#define LOTRCHESS_TOGGLE_BUTTON_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class ToggleButton {
private:
    std::vector<std::string> option_names_;
    std::vector<sf::Text> option_text_;
    std::vector<sf::RectangleShape> rects_;
    sf::FloatRect bounding_box_;
    sf::Color selected_color_;
    sf::Color hovering_color_;
    sf::Color default_color_;
    sf::Color selected_font_color_;
    sf::Color hovering_font_color_;
    sf::Color default_font_color_;
    sf::Font font_;


public:
    explicit ToggleButton(std::vector<std::string> option_names,
                        const sf::Color& selected_color,
                        const sf::Color& hovering_color,
                        const sf::Color& default_color,
                        const sf::Color& selected_font_color,
                        const sf::Color& hovering_font_color,
                        const sf::Color& default_font_color) :
                            option_names_(std::move(option_names)),
                            selected_color_(selected_color),
                            hovering_color_(hovering_color),
                            default_color_(default_color),
                            selected_font_color_(selected_font_color),
                            hovering_font_color_(hovering_font_color),
                            default_font_color_(default_font_color) {
        option_text_.reserve(option_names_.size());
        rects_.reserve(option_names_.size());
    }

    void set_bounding_box(const sf::FloatRect& rect) {
        bounding_box_ = rect;
    }

    void set_font(const sf::Font& font) {
        font_ = font;
    }

    void create_toggle() {
        auto num_options = static_cast<float>(option_names_.size());
        auto width_per_rect = bounding_box_.width / num_options;
        sf::Vector2f rect_size(width_per_rect, bounding_box_.height);


        for (std::size_t i = 0; i < rects_.size(); ++i) {
            rects_[i].setSize(rect_size);
            rects_[i].setFillColor(default_color_);

            option_text_[i].setFillColor(default_color_);
            option_text_[i].setString(option_names_[i]);
            auto local_bounds = rects_[i].getLocalBounds();
            option_text_[i].setPosition(local_bounds.left + local_bounds.width / 3, local_bounds.top + local_bounds.height / 2);
        }
    }

    void reset_colors() {
        for (auto& rect : rects_) {
            rect.setFillColor(default_color_);
        }
        for (auto& o : option_text_) {
            o.setFillColor(default_color_);
        }
    }

    void update_selected(const std::size_t index) {
        reset_colors();
        rects_[index].setFillColor(selected_color_);
        option_text_[index].setFillColor(selected_color_);
    }

    void update_hovering(const std::size_t index) {
        reset_colors();
        rects_[index].setFillColor(hovering_color_);
        option_text_[index].setFillColor(hovering_color_);
    }

    void draw(sf::RenderWindow& window) {
//        for (const auto& rect : rects_) {
//            window.draw(rect);
//        }

        for (const auto& t : option_text_) {
            window.draw(t);
        }
    }


};

#endif // LOTRCHESS_TOGGLE_BUTTON_HPP
