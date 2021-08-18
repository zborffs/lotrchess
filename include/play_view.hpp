#ifndef LOTRCHESS_PLAY_VIEW_HPP
#define LOTRCHESS_PLAY_VIEW_HPP

/// third party libs
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <spdlog/spdlog.h>

/// stl includes
#include <vector>
#include <unordered_map>

/// project includes
#include "defines.hpp"
#include "play_controller.hpp"

/**
 * class responsible for holding the graphics objects related to the board
 */
class PlayView {
private:
    sf::Texture board_texture_; // board texture (.png)
    sf::Sprite board_; // board sprite (drawable object in SFML)
    sf::Texture one_ring_texture_; // one ring texture (.png)
    sf::Sprite one_ring_sprite_; // one ring sprite (drawable obj in SFML)
    std::unordered_map<char, sf::Texture> piece_texture_map_; // texture for each type of piece
    std::vector<sf::Sprite> piece_sprites_; // pieces on the board
    std::vector<sf::Sprite> highlighted_sqs_; // highlighted squares
    sf::Texture white_victory_texture_;
    sf::Sprite white_victory_;
    sf::Texture black_victory_texture_;
    sf::Sprite black_victory_;
    sf::Texture draw_result_texture_; // draw and stalemate texture
    sf::Sprite draw_result_; // draw or stalemate result sprite
    ResultFlag result_;

    sf::Text result_text_;
    sf::Text return_text_;
    sf::Font font_{};
    const unsigned int small_font_size{42};
    const unsigned int medium_font_size{43};
    const unsigned int large_font_size{72};

public:
    PlayView(PlayController &board, sf::Vector2f board_offset, const std::string &board_texture_path,
             const std::string &one_ring_path, const std::unordered_map<char, std::string> &texture_path_map);

    void update_pieces(PlayController &board);

    void draw(sf::RenderWindow &draw);

    /**
     * the bounding box of the board object
     * @return returns a Vector2f of the board sprite
     */
    [[nodiscard]] inline sf::Vector2f board_offset() noexcept {
        return board_.getPosition();
    }

    /**
     * the bounding box of the board object
     * @return returns a Vector2f of the board sprite
     */
    [[nodiscard]] inline sf::Vector2f board_offset() const noexcept {
        return board_.getPosition();
    }

    bool in_return(int x, int y) {
        auto min_x = return_text_.getGlobalBounds().left;
        auto max_x = min_x + return_text_.getGlobalBounds().width;
        auto min_y = return_text_.getGlobalBounds().top;
        auto max_y = min_y + return_text_.getGlobalBounds().height;
        auto xf = static_cast<float>(x);
        auto yf = static_cast<float>(y);

        return xf <= max_x && xf >= min_x && yf <= max_y && yf >= min_y;
    }

    bool in_board(int x, int y);

    void update_return_selected(PlayController& controller);
};

#endif // LOTRCHESS_PLAY_VIEW_HPP
