#include "play_screen.hpp"

/**
 * responsible for processing the event when in the "Play" Screen
 * @param event
 */
void PlayScreen::process_event(sf::Event &event) {
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
                spdlog::info("R was pressed...");
                controller_.rotate_board(); // update the model
                board_view_.update_pieces(controller_); // update the view's understanding of the model
            } else if (event.key.code == sf::Keyboard::A) {
                // if the user pressed "A", then play white startup music
                spdlog::info("A was pressed...");
                context_->music_player_.play_startup_music(WHITE);
            } else if (event.key.code == sf::Keyboard::S) {
                // if the user pressed "S", then play black startup music
                spdlog::info("S was pressed...");
                context_->music_player_.play_startup_music(BLACK);
            } else if (event.key.code == sf::Keyboard::D) {
                // if the user pressed "D", then play white victory music
                spdlog::info("D was pressed...");
                context_->music_player_.play_victory_music(WHITE);
            } else if (event.key.code == sf::Keyboard::F) {
                // if the user pressed "F", then play black victory music
                spdlog::info("F was pressed...");
                context_->music_player_.play_victory_music(BLACK);
            } else if (event.key.code == sf::Keyboard::Escape) {
                // if the user pressed "Escape", then go to the main menu
                spdlog::info("ESC was pressed...");
                context_->transition_to(new SplashScreen());
            }
            break;
        }
        case sf::Event::MouseMoved: {
            int x = event.mouseMove.x;
            int y = event.mouseMove.y;

            if (board_view_.in_board(x, y)) {
                if (board_view_.in_return(x, y)) {
                    controller_.set_return_selected(true);
                    board_view_.update_return_selected(controller_);
                } else {
                    controller_.set_return_selected(false);
                    board_view_.update_return_selected(controller_);
                }
            }
            break;
        }
        case sf::Event::MouseButtonPressed: {
            int x = event.mouseButton.x;
            int y = event.mouseButton.y;

            if (board_view_.in_board(x, y)) {
                // if the user pressed on the mouse inside the board's bounding box, then figure out what square was clicked
                sf::Vector2f board_view_offset = board_view_.board_offset();

                if (controller_.result_flag() == DRAW || controller_.result_flag() == WHITE_VICTORY || controller_.result_flag() == BLACK_VICTORY) {
                    if (board_view_.in_return(x, y)) {
                        context_->transition_to(new SplashScreen());
                        return;
                    }
                }

                if (controller_.player_promoting()) {
                    spdlog::info("Screen realizes that the player is trying to promote a pawn");
//                    board_view_.update_pieces(controller_);
                    return;
                }


                int col = static_cast<int>((static_cast<float>(x) - board_view_offset.x) / 100);
                int row = static_cast<int>((static_cast<float>(y) - board_view_offset.y) / 100);

                // to determine what the square was, we need to check if the board is reversed or not on the screen
                Square_t sq{Square::EP_DEFAULT};
                if (controller_.as_white()) {
                    sq = static_cast<Square_t>(col + (7 - row) * 8);
                } else {
                    sq = static_cast<Square_t>((7 - col) + row * 8);
                }

                if (controller_.piece_selected()) {
                    if (controller_.move_is_legal(sq)) {
                        controller_.make_move(sq);
                        controller_.clear_highlights();
                        board_view_.update_pieces(controller_);

                        switch (controller_.result_flag()) {
                            case NO_RESULT:
                                context_->music_player_.play_piece_moved_sfx();
                                break;
                            case DRAW:
                                spdlog::info("Draw");
                                break;
                            case STALEMATE:
                                spdlog::info("Stalemate");
                                break;
                            case WHITE_VICTORY:
                                spdlog::info("White wins");
                                context_->music_player_.play_victory_music(WHITE);
                                break;
                            case BLACK_VICTORY:
                                spdlog::info("Black Wins");
                                context_->music_player_.play_victory_music(BLACK);
                                break;
                        }
                    } else if (controller_.valid_highlight(sq)) {
                        controller_.add_highlight(sq); // add a highlight on the square that was clicked
                        board_view_.update_pieces(controller_); // update the pieces
                    } else {
                        controller_.clear_highlights(); // add a highlight on the square that was clicked
                        board_view_.update_pieces(controller_); // update the pieces
                    }
                } else {
                    if (controller_.valid_highlight(sq)) {
                        controller_.add_highlight(sq); // add a highlight on the square that was clicked
                        board_view_.update_pieces(controller_); // update the pieces
                    } else {
                        controller_.clear_highlights(); // add a highlight on the square that was clicked
                        board_view_.update_pieces(controller_); // update the pieces
                    }
                }
            }
        }
        default:
            spdlog::trace("Unhanded event type in PlayScreen...");
    }
}

void PlayScreen::draw(sf::RenderWindow &window) {
    board_view_.draw(window);
}

void PlayScreen::update() {
//    Screen::update();
    if (controller_.check_result()) {
        // check to see whether the controller has a result from the engine_io_ object
        board_view_.update_pieces(controller_); // update the board if there is a result


        // play the corresponding must / show the corresponding screen to accord with the game result
        switch (controller_.result_flag()) {
            case NO_RESULT:
                context_->music_player_.play_piece_moved_sfx();
                break;
            case DRAW:
                spdlog::info("Draw");
                break;
            case STALEMATE:
                spdlog::info("Stalemate");
                break;
            case WHITE_VICTORY:
                spdlog::info("White wins");
                context_->music_player_.play_victory_music(WHITE);
                break;
            case BLACK_VICTORY:
                spdlog::info("Black Wins");
                context_->music_player_.play_victory_music(BLACK);
                break;
        }
    }
}
