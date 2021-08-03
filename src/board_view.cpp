#include "board_view.hpp"

BoardView::BoardView(const Board& board, sf::Vector2f board_offset, const std::string& board_texture_path, const std::unordered_map<char, std::string>& texture_path_map) {

    // initialize board texture
    if (!board_texture_.loadFromFile(board_texture_path)) {
        spdlog::error("Failed to load the board texture in BoardView constructor...");
    }
    board_.setTexture(board_texture_);
    board_.setPosition(board_offset); // absolute position

    // initialize piece_texture_map_; maps characters to sf::Textures
    for (const auto& kv_pair : texture_path_map) {
        // load the file in the path, then insert the texture to the map
        sf::Texture texture;
        if (!texture.loadFromFile(kv_pair.second)) {
            spdlog::error("Failed to load the {} texture in BoardView constructor...", kv_pair.second);
        }
        piece_texture_map_[kv_pair.first] = texture;
    }

    piece_sprites_.reserve(8 * 4); // reserve the max inserts

    // initialize piece_sprites_ (where do which sprites go?)
    update_pieces(board);
}

void BoardView::update_pieces(const Board& board) {
    // initialize piece_sprites_ (where do which sprites go?)
    for (Square_t sq = Square::A1; sq <= Square::H8; ++ sq) {
        // add the corresponding texture, then put on the corresponding square
        piece_sprites_.emplace_back(piece_texture_map_[board.piece_at(static_cast<Square>(sq))]);
        piece_sprites_.back().setPosition(board_.getPosition());
        if (as_white) {
            piece_sprites_.back().move((sq % 8) * 100, (7 - sq / 8) * 100);
        } else {
            piece_sprites_.back().move((sq % 8) * 100, sq / 8 * 100);
        }
    }
}

void BoardView::draw(sf::RenderWindow& window) {
    window.draw(board_);
    for (const auto& sprite : piece_sprites_) {
        window.draw(sprite);
    }

}
