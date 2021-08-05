#include "play_mode.hpp"

void PlayScreen::process_event(sf::Event &event) {
    // poll every event that has occurred
    ImGui::SFML::ProcessEvent(event);

    switch (event.type) {
    case sf::Event::Closed: {
        // if it was a close window event, then close the window
        context_->window_.close();
        break;
    }
    case sf::Event::KeyPressed: {
        if (event.key.code == sf::Keyboard::R) {
            // if the user pressed 'R' then reverse the board
            spdlog::info("R was pressed");
            board_.switch_sides(); // update the model
            board_view_.update_pieces(board_); // update the view's understanding of the model
        } else if (event.key.code == sf::Keyboard::A) {
            spdlog::info("A was pressed...");
            context_->music_player_.play_startup_music(WHITE);
        } else if (event.key.code == sf::Keyboard::S) {
            spdlog::info("S was pressed...");
            context_->music_player_.play_startup_music(BLACK);
        } else if (event.key.code == sf::Keyboard::D) {
            spdlog::info("D was pressed...");
            context_->music_player_.play_victory_music(WHITE);
        } else if (event.key.code == sf::Keyboard::F) {
            spdlog::info("F was pressed...");
            context_->music_player_.play_victory_music(BLACK);
        } else if (event.key.code == sf::Keyboard::Escape) {
            spdlog::info("ESC was pressed...");
            context_->transition_to(new SplashScreen());
        }
    }
    case sf::Event::MouseButtonPressed: {
        int x = event.mouseButton.x;
        int y = event.mouseButton.y;

        if (board_view_.in_board(x, y)) {
            // if the user pressed on the mouse inside the board's bounding box, then figure out what square was clicked
            sf::Vector2f board_view_offset = board_view_.board_offset();
            int col = static_cast<int>((static_cast<float>(x) - board_view_offset.x) / 100);
            int row = static_cast<int>((static_cast<float>(y) - board_view_offset.y) / 100);

            // to determine what the square was, we need to check if the board is reversed or not on the screen
            Square_t sq {Square::EP_DEFAULT};
            if (board_.as_white()) {
                sq = static_cast<Square_t>(col + (7 - row) * 8);
            } else {
                sq = static_cast<Square_t>((7 - col) + row * 8);
            }

            if (board_.valid_highlight(sq)) {
                board_.add_highlight(sq); // add a highlight on the square that was clicked
                board_view_.update_pieces(board_); // update the pieces
            } else {
                board_.clear_highlights(); // add a highlight on the square that was clicked
                board_view_.update_pieces(board_); // update the pieces
            }
        }
    }
    default:
        spdlog::trace("Unhanded event type in PlayScreen");
    }
}
void PlayScreen::draw(sf::RenderWindow &window) {
    board_view_.draw(window);
}
