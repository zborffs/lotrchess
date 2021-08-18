#ifndef LOTRCHESS_BATTLE_SETUP_VIEW_HPP
#define LOTRCHESS_BATTLE_SETUP_VIEW_HPP

/// third party includes
#include <SFML/Graphics.hpp>

/// stl includes
#include <array>

/// lotrchess includes
#include "battle_setup_controller.hpp"
#include "extern.hpp"
#include "toggle_button.hpp"

/**
 * The "View" (in context of MVC) for the Battle Setup Screen
 * - responsible for:
 *   1. maintaining all graphics objects when in the Battle Setup screen
 *   2. transforming graphics data given the control parameters in the
 *   BattleSetupController class
 */
class BattleSetupView {
    const sf::Vector2f bg_offset_{10., 10.};
    const std::array<std::string, 2> option_string_{"Color", "Difficulty"};
    const std::array<std::string, 2> color_option_string_{"White", "Black"};
    const std::array<std::string, 3> difficulty_option_string_{"Tough", "Hard", "Impossible"};
    sf::Texture bg_texture_;
    sf::Sprite bg_;
    sf::Font font_{};
    sf::Text title_text_;
    std::array<sf::Text, 2> option_text_;
    std::array<sf::Text, 2> color_option_text_;
    std::array<sf::Text, 3> difficulty_option_text_;
    sf::Text select_;

public:
    BattleSetupView() {
        // configure the background
        bg_texture_.loadFromFile(SPLASH_BG_PATH);
        bg_.setTexture(bg_texture_);
        bg_.setPosition(bg_offset_);

        // configure the font
        font_.loadFromFile(FONT_PATH);

        // configure the title
        title_text_.setFont(font_);
        title_text_.setString("Custom Battle");
        title_text_.setCharacterSize(60); // arbitrary
        title_text_.setPosition(bg_offset_);
        title_text_.move(bg_.getGlobalBounds().width / 5, bg_.getGlobalBounds().height / 5);

        auto pos = title_text_.findCharacterPos(0);

        float vert_offset{175};
        std::size_t index{1};
        for (auto &option : option_text_) {
            option.setFont(font_);
            option.setCharacterSize(42); // arbitrary
            option.setString(option_string_[index - 1]);
            option.setPosition(pos);
            option.move(0, vert_offset * static_cast<float>(index));
            ++index;
        }

        index = 1;
        for (auto& option : color_option_text_) {
            option.setFont(font_);
            option.setCharacterSize(32); // arbitrary
            option.setString(color_option_string_[index - 1]);
            option.setPosition(pos);
            option.move(static_cast<float>(index) * 200, vert_offset + 100.);
            ++index;
        }

        index = 1;
        for (auto& option: difficulty_option_text_) {
            option.setFont(font_);
            option.setCharacterSize(32); // arbitrary
            option.setString(difficulty_option_string_[index - 1]);
            option.setPosition(pos);
            option.move(static_cast<float>(index) * 200, vert_offset * 2.0 + 100.);
            ++index;
        }

        select_.setFont(font_);
        select_.setCharacterSize(32);
        select_.setString("Select");
        select_.setPosition(bg_offset_);
        select_.move(bg_.getGlobalBounds().width * 2 / 3, bg_.getGlobalBounds().height * 5 / 6);
    }

    bool in_white(int x, int y) {
        auto min_x = color_option_text_[WHITE].getGlobalBounds().left;
        auto max_x = min_x + color_option_text_[WHITE].getGlobalBounds().width;
        auto min_y = color_option_text_[WHITE].getGlobalBounds().top;
        auto max_y = min_y + color_option_text_[WHITE].getGlobalBounds().height;
        auto xf = static_cast<float>(x);
        auto yf = static_cast<float>(y);

        return xf <= max_x && xf >= min_x && yf <= max_y && yf >= min_y;
    }

    bool in_black(int x, int y) {
        auto min_x = color_option_text_[BLACK].getGlobalBounds().left;
        auto max_x = min_x + color_option_text_[BLACK].getGlobalBounds().width;
        auto min_y = color_option_text_[BLACK].getGlobalBounds().top;
        auto max_y = min_y + color_option_text_[BLACK].getGlobalBounds().height;
        auto xf = static_cast<float>(x);
        auto yf = static_cast<float>(y);

        return xf <= max_x && xf >= min_x && yf <= max_y && yf >= min_y;
    }

    bool in_selected(int x, int y) {
        auto min_x = select_.getGlobalBounds().left;
        auto max_x = min_x + select_.getGlobalBounds().width;
        auto min_y = select_.getGlobalBounds().top;
        auto max_y = min_y + select_.getGlobalBounds().height;
        auto xf = static_cast<float>(x);
        auto yf = static_cast<float>(y);

        return xf <= max_x && xf >= min_x && yf <= max_y && yf >= min_y;
    }

    void update_options(BattleSetupController& dialog) {
        for (auto& m : color_option_text_) {
            m.setFillColor(DEFAULT_FONT_COLOR);
        }
        if (dialog.human_color() != Color::BOTH) {
            color_option_text_[dialog.human_color()].setFillColor(SELECTED_FONT_COLOR);
            select_.setFillColor(SELECTED_FONT_COLOR);
        }
    }

    void draw(sf::RenderWindow &window) {
        window.draw(bg_);
        window.draw(title_text_);
        window.draw(select_);

        for (const auto &o : option_text_) {
            window.draw(o);
        }

        for (const auto& o : color_option_text_) {
            window.draw(o);
        }

        for (const auto& o : difficulty_option_text_) {
            window.draw(o);
        }
    }
};

#endif // LOTRCHESS_BATTLE_SETUP_VIEW_HPP
