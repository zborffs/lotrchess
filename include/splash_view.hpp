#ifndef LOTRCHESS_SPLASH_VIEW_HPP
#define LOTRCHESS_SPLASH_VIEW_HPP

/// third party includes
#include <SFML/Graphics.hpp>

/// stl includes
#include <array>

/// project includes
#include "extern.hpp"
#include "splash_controller.hpp"

class SplashView {
private:
    const sf::Vector2f bg_offset_{10., 10.};
    const std::array<std::string, 4> mode_string_{"Campaign", "Custom Battle", "Replay", "Quit"};
    sf::Texture bg_texture_;
    sf::Sprite bg_;
    sf::Font font_{};
    sf::Text title_text_;
    std::array<sf::Text, 4> mode_selection_text_;
    const unsigned int small_font_size{42};
    const unsigned int medium_font_size{43};
    const unsigned int large_font_size{72};

public:
  SplashView()  {
        // configure the background
        bg_texture_.loadFromFile(SPLASH_BG_PATH);
        bg_.setTexture(bg_texture_);
        bg_.setPosition(bg_offset_);

        // configure the font
        font_.loadFromFile(FONT_PATH);

        // configure the title
        title_text_.setFont(font_);
        title_text_.setString("Lord of the Rings\n            Chess");
        title_text_.setCharacterSize(large_font_size); // arbitrary
        title_text_.setPosition(bg_offset_);
        title_text_.move(bg_.getGlobalBounds().width / 5, bg_.getGlobalBounds().height / 5);

        auto first_char_title_pos = title_text_.findCharacterPos(0);
        auto last_line_char_title_pos = title_text_.findCharacterPos(16);
        auto last_char_title_pos = title_text_.findCharacterPos(title_text_.getString().getSize());
        sf::Vector2f pos((last_line_char_title_pos.x + first_char_title_pos.x) / 2, last_char_title_pos.y);

        float vert_offset{115};
        std::size_t index{1};
        for (auto& mode : mode_selection_text_) {
            mode.setFont(font_);
            mode.setCharacterSize(small_font_size); // arbitrary
            mode.setString(mode_string_[index - 1]);
            mode.setPosition(pos);
            auto curr_mode_x_offset = (mode.findCharacterPos(mode.getString().getSize()).x - mode.findCharacterPos(0).x) / 2;
            mode.move(-curr_mode_x_offset, vert_offset * static_cast<float>(index));
            ++index;
        }
    }

    void update_options(SplashController & dialog) {
        for (auto& m : mode_selection_text_) {
            m.setFillColor(DEFAULT_FONT_COLOR);
            m.setCharacterSize(small_font_size);
        }

        if (dialog.selection() != None) {
            mode_selection_text_[dialog.selection()].setFillColor(SELECTED_FONT_COLOR);
            mode_selection_text_[dialog.selection()].setCharacterSize(medium_font_size);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(bg_);
        window.draw(title_text_);

        for (const auto& m : mode_selection_text_) {
            window.draw(m);
        }
    }

    bool in_campaign(int x, int y) {
        auto min_x = mode_selection_text_[SplashSelector::Campaign].getGlobalBounds().left;
        auto max_x = min_x + mode_selection_text_[SplashSelector::Campaign].getGlobalBounds().width;
        auto min_y = mode_selection_text_[SplashSelector::Campaign].getGlobalBounds().top;
        auto max_y = min_y + mode_selection_text_[SplashSelector::Campaign].getGlobalBounds().height;
        auto xf = static_cast<float>(x);
        auto yf = static_cast<float>(y);

        return xf <= max_x && xf >= min_x && yf <= max_y && yf >= min_y;
    }
    bool in_battle(int x, int y) {
        auto min_x = mode_selection_text_[SplashSelector::CustomBattle].getGlobalBounds().left;
        auto max_x = min_x + mode_selection_text_[SplashSelector::CustomBattle].getGlobalBounds().width;
        auto min_y = mode_selection_text_[SplashSelector::CustomBattle].getGlobalBounds().top;
        auto max_y = min_y + mode_selection_text_[SplashSelector::CustomBattle].getGlobalBounds().height;
        auto xf = static_cast<float>(x);
        auto yf = static_cast<float>(y);

        return xf <= max_x && xf >= min_x && yf <= max_y && yf >= min_y;
    }
    bool in_replay(int x, int y) {
        auto min_x = mode_selection_text_[SplashSelector::Replay].getGlobalBounds().left;
        auto max_x = min_x + mode_selection_text_[SplashSelector::Replay].getGlobalBounds().width;
        auto min_y = mode_selection_text_[SplashSelector::Replay].getGlobalBounds().top;
        auto max_y = min_y + mode_selection_text_[SplashSelector::Replay].getGlobalBounds().height;
        auto xf = static_cast<float>(x);
        auto yf = static_cast<float>(y);

        return xf <= max_x && xf >= min_x && yf <= max_y && yf >= min_y;
    }
    bool in_quit(int x, int y) {
        auto min_x = mode_selection_text_[SplashSelector::Quit].getGlobalBounds().left;
        auto max_x = min_x + mode_selection_text_[SplashSelector::Quit].getGlobalBounds().width;
        auto min_y = mode_selection_text_[SplashSelector::Quit].getGlobalBounds().top;
        auto max_y = min_y + mode_selection_text_[SplashSelector::Quit].getGlobalBounds().height;
        auto xf = static_cast<float>(x);
        auto yf = static_cast<float>(y);

        return xf <= max_x && xf >= min_x && yf <= max_y && yf >= min_y;
    }
};

#endif // LOTRCHESS_SPLASH_VIEW_HPP
