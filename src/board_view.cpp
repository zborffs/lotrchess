#include "board_view.hpp"

BoardView::BoardView(const Board& board, sf::Vector2f board_offset, const std::string& board_texture_path, const std::string& one_ring_path, const std::unordered_map<char, std::string>& texture_path_map) : highlighted_sqs_({}){

    // initialize board texture
    if (!board_texture_.loadFromFile(board_texture_path)) {
        spdlog::error("Failed to load the board texture in BoardView constructor...");
    }
    board_.setTexture(board_texture_);
    board_.setPosition(board_offset); // absolute position

    // initialize one ring texture
    if (!one_ring_texture_.loadFromFile(one_ring_path)) {
        spdlog::error("Failed to load the one ring texture in BoardView constructor...");
    }
    one_ring_sprite_.setTexture(one_ring_texture_);

    // initialize piece_texture_map_; maps characters to sf::Textures
    for (const auto& kv_pair : texture_path_map) {
        // load the file in the path, then insert the texture to the map
        sf::Texture texture;
        if (!texture.loadFromFile(kv_pair.second)) {
            spdlog::error("Failed to load the {} texture in BoardView constructor...", kv_pair.second);
        }
        piece_texture_map_[kv_pair.first] = texture;
    }

    update_pieces(board);
}

void BoardView::update_pieces(const Board& board) {
    // initialize piece_sprites_ (where do which sprites go?)
    piece_sprites_.clear();
    highlighted_sqs_.clear();
    highlighted_sqs_.reserve(10);
    piece_sprites_.reserve(8 * 4); // reserve the max inserts
    for (Square_t sq = Square::A1; sq <= Square::H8; ++sq) {
        // add the corresponding texture, then put on the corresponding square
        piece_sprites_.emplace_back(piece_texture_map_[board.piece_at(static_cast<Square>(sq))]);
        piece_sprites_.back().setPosition(board_.getPosition()); // bias

        bool is_highlighted = board.is_highlighted(sq);
        if (is_highlighted) {
            highlighted_sqs_.emplace_back(one_ring_sprite_);
            highlighted_sqs_.back().setPosition(board_.getPosition()); // bias
            auto local_bounds = highlighted_sqs_.back().getLocalBounds();
            highlighted_sqs_.back().setOrigin(local_bounds.width / 2,local_bounds.height / 2);
            highlighted_sqs_.back().move(local_bounds.width / 2,local_bounds.height / 2);
        }

        if (board.as_white()) {
            auto x_offset = static_cast<float>((sq % 8) * 100);
            auto y_offset = static_cast<float>((7 - sq / 8) * 100);
            piece_sprites_.back().move(x_offset, y_offset);
            if (is_highlighted) {
                highlighted_sqs_.back().move(x_offset, y_offset);
            }
        } else {
            auto x_offset = static_cast<float>((7 - (sq % 8)) * 100);
            auto y_offset = static_cast<float>(sq / 8 * 100);
            piece_sprites_.back().move(x_offset, y_offset);
            if (is_highlighted) {
                highlighted_sqs_.back().move(x_offset, y_offset);
            }
        }
    }
}

void BoardView::draw(sf::RenderWindow& window) {
    window.draw(board_);
    for (const auto& sprite : piece_sprites_) {
        window.draw(sprite);
    }

    for (auto& sq : highlighted_sqs_) {
        sq.rotate(0.9f);
        window.draw(sq);
    }
}

/**
 * returns whether or not the given coordinates are within the board sprite
 * rectangle
 * @param x coordinate along x-axis in pixels
 * @param y coordinate along y-axis in pixels
 * @return true if inside bounding box of chessboard sprite, false otherwise
 */
bool BoardView::in_board(int x, int y) {
    // compute bounding box
    auto min_y = board_.getGlobalBounds().top;
    auto min_x = board_.getGlobalBounds().left;
    auto max_y = min_y + board_.getGlobalBounds().height;
    auto max_x = min_x + board_.getGlobalBounds().width;

    // convert the coordinates to floats so that they may be compared
    auto xf = static_cast<float>(x);
    auto yf = static_cast<float>(y);

    return xf < max_x && xf > min_x && yf < max_y && yf > min_y;
}