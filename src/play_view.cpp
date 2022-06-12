#include "play_view.hpp"

PlayView::PlayView(PlayController& board, sf::Vector2f board_offset, const std::string& board_texture_path, const std::string& one_ring_path, const std::unordered_map<char, std::string>& texture_path_map) : highlighted_sqs_({}){

    // initialize board texture
    if (!board_texture_.loadFromFile(board_texture_path)) {
        spdlog::error("Failed to load the board texture in PlayView constructor...");
    }
    board_.setTexture(board_texture_);
    board_.setPosition(board_offset); // absolute position

    if (!bg_texture_.loadFromFile(PLAY_BG_PATH)){
        spdlog::error("Failed to load the playscreen background texture in the PlayView constructor...");
    }
    bg_.setTexture(bg_texture_);
    bg_.scale(0.50, 0.50); // 0.80, 0.80
    bg_.move(425.0, 75.0); // -50, -150

    // initialize one ring texture
    if (!one_ring_texture_.loadFromFile(one_ring_path)) {
        spdlog::error("Failed to load the one ring texture in PlayView constructor...");
    }
    one_ring_sprite_.setTexture(one_ring_texture_);

    // initialize piece_texture_map_; maps characters to sf::Textures
    for (const auto& kv_pair : texture_path_map) {
        // load the file in the path, then insert the texture to the map
        sf::Texture texture;
        if (!texture.loadFromFile(kv_pair.second)) {
            spdlog::error("Failed to load the {} texture in PlayView constructor...", kv_pair.second);
        }
        piece_texture_map_[kv_pair.first] = texture;
    }

    if (!white_victory_texture_.loadFromFile(WHITE_VICTORY_TEXTURE)) {
        spdlog::error("Failed to load the white victory texture");
    }
    white_victory_.setTexture(white_victory_texture_);
    white_victory_.setPosition(board_offset);
    white_victory_.move(100,100);

    if (!black_victory_texture_.loadFromFile(BLACK_VICTORY_TEXTURE)) {
        spdlog::error("Failed to load the black victory texture");
    }
    black_victory_.setTexture(black_victory_texture_);
    black_victory_.setPosition(board_offset);
    black_victory_.move(100,100);

    if (!draw_result_texture_.loadFromFile(DRAW_RESULT_TEXTURE)) {
        spdlog::error("Failed to load the draw result texture");
    }
    draw_result_.setTexture(draw_result_texture_);
    draw_result_.setPosition(board_offset);
    draw_result_.move(100,100);

    // configure the font
    font_.loadFromFile(FONT_PATH);

    // configure the title
    result_text_.setFont(font_);
    result_text_.setCharacterSize(large_font_size); // arbitrary
    result_text_.setPosition(board_offset);
    result_text_.move(225, 300);

    return_text_.setFont(font_);
    return_text_.setCharacterSize(32);
    return_text_.setPosition(board_offset);
    return_text_.setString("Return");
    return_text_.move(600 - 20, 650);

    update_pieces(board);
}

void PlayView::update_pieces(PlayController & board) {
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

    result_ = board.result_flag();

    if (board.player_promoting()) {
        spdlog::info("BoardView knows to draw promotion!");
    }
}

void PlayView::draw(sf::RenderWindow& window) {
    window.draw(bg_);
    window.draw(board_); // draw the board

    // draw highlights on the board
    for (auto& sq : highlighted_sqs_) {
        sq.rotate(1.0f); // 0.5f
        window.draw(sq);
    }

    // draw the pieces on the board
    for (const auto& sprite : piece_sprites_) {
        window.draw(sprite);
    }

    switch (result_) {
        case NO_RESULT:
            break;
        case DRAW:
            window.draw(draw_result_);
            result_text_.setString("Draw");
            window.draw(result_text_);
            window.draw(return_text_);
            break;
        case STALEMATE:
            window.draw(draw_result_);
            result_text_.setString("Stalemate");
            window.draw(result_text_);
            window.draw(return_text_);
            break;
        case WHITE_VICTORY:
            window.draw(white_victory_);
            result_text_.setString("White Wins");
            window.draw(result_text_);
            window.draw(return_text_);
            break;
        case BLACK_VICTORY:
            window.draw(black_victory_);
            result_text_.setString("Black Wins");
            window.draw(result_text_);
            window.draw(return_text_);
            break;
    }
}

/**
 * returns whether or not the given coordinates are within the board sprite
 * rectangle
 * @param x coordinate along x-axis in pixels
 * @param y coordinate along y-axis in pixels
 * @return true if inside bounding box of chessboard sprite, false otherwise
 */
bool PlayView::in_board(int x, int y) {
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

void PlayView::update_return_selected(PlayController& controller) {
    if (controller.return_selected()) {
        return_text_.setFillColor(SELECTED_FONT_COLOR);
    } else {
        return_text_.setFillColor(DEFAULT_FONT_COLOR);
    }
}
