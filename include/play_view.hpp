#ifndef LOTRCHESS_PLAY_VIEW_HPP
#define LOTRCHESS_PLAY_VIEW_HPP

/// third party libs
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
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

public:
  PlayView(const PlayController & board, sf::Vector2f board_offset, const std::string& board_texture_path, const std::string& one_ring_path, const std::unordered_map<char, std::string>& texture_path_map);

    void update_pieces(const PlayController & board);
    void draw(sf::RenderWindow& draw);

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

    bool in_board(int x, int y);
};

#endif // LOTRCHESS_PLAY_VIEW_HPP
