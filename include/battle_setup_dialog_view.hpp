#ifndef LOTRCHESS_BATTLE_SETUP_DIALOG_VIEW_HPP
#define LOTRCHESS_BATTLE_SETUP_DIALOG_VIEW_HPP

#include "battle_setup_dialog.hpp"
#include "toggle_button.hpp"
#include <SFML/Graphics.hpp>
#include <array>

class BattleSetupDialogView {
//    const sf::Color default_color_{10, 10, 10, 255};
//    const sf::Color selected_color_{255, 215,0, 255};
//    const sf::Color hovering_color_{240, 200,0, 255};
//    const sf::Color default_font_color_{255, 255, 255, 255};
//    const sf::Color hovering_font_color_{255, 255, 255, 255};
//    const sf::Color selected_font_color_{255, 255, 255, 255};

    const sf::Vector2f bg_offset_{10., 10.};
    const std::string font_path_{"../../res/font/lotr_font.ttf"};
    const std::string bg_path_{"../../res/img/splash_bg.jpg"};
    const std::array<std::string, 2> option_string_{"Color", "Strength"};
    sf::Texture bg_texture_;
    sf::Sprite bg_;
    sf::Font font_{};
    sf::Text title_text_;
    std::array<sf::Text, 2> option_text_;

public:
    BattleSetupDialogView() {
        // configure the background
        bg_texture_.loadFromFile(bg_path_);
        bg_.setTexture(bg_texture_);
        bg_.setPosition(bg_offset_);

        // configure the font
        font_.loadFromFile(font_path_);

        // configure the title
        title_text_.setFont(font_);
        title_text_.setString("Custom Battle");
        title_text_.setCharacterSize(60); // arbitrary
        title_text_.setPosition(bg_offset_);
        title_text_.move(bg_.getGlobalBounds().width / 5, bg_.getGlobalBounds().height / 5);

        auto pos = title_text_.findCharacterPos(0);

        float vert_offset{175};
        std::size_t index{1};
        for (auto&option : option_text_) {
            option.setFont(font_);
            option.setCharacterSize(42); // arbitrary
            option.setString(option_string_[index - 1]);
            option.setPosition(pos);
            option.move(0, vert_offset * static_cast<float>(index));
            ++index;
        }
    }

//    void update_options(BattleSetupDialog& dialog) {
//        for (auto& m : mode_selection_text_) {
//            m.setFillColor(default_color_);
//        }
//        if (dialog.selection() != None) {
//            mode_selection_text_[dialog.selection()].setFillColor(selected_color_);
//        }
//    }

    void draw(sf::RenderWindow& window) {
        window.draw(bg_);
        window.draw(title_text_);

        for (const auto& o : option_text_) {
            window.draw(o);
        }
    }
};

#endif // LOTRCHESS_BATTLE_SETUP_DIALOG_VIEW_HPP
