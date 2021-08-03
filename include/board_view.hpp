#ifndef LOTRCHESS_BOARD_VIEW_HPP
#define LOTRCHESS_BOARD_VIEW_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics/RenderWindow.hpp>
#include "defines.hpp"
#include "board.hpp"
#include <spdlog/spdlog.h>

class BoardView {
private:
    sf::Texture board_texture_;
    sf::Sprite board_; // actual board
    std::unordered_map<char, sf::Texture> piece_texture_map_;
    std::vector<sf::Sprite> piece_sprites_; // pieces on the board
    bool as_white{true};

public:
    explicit BoardView(const Board& board, sf::Vector2f board_offset, const std::string& board_texture_path, const std::unordered_map<char, std::string>& texture_path_map);

    void update_pieces(const Board& board);
    void draw(sf::RenderWindow& draw);
};

#endif // LOTRCHESS_BOARD_VIEW_HPP
