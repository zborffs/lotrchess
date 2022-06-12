#include "battle_setup_screen.hpp"
void BattleSetupScreen::process_event(sf::Event& event) {
    ImGui::SFML::ProcessEvent(event);

    switch (event.type) {
    case sf::Event::Closed: {
        // if it was a close window event, then close the window
        context_->window_.close();
        break;
    }
    case sf::Event::MouseButtonPressed: {
        auto x = event.mouseButton.x;
        auto y = event.mouseButton.y;
        if (view_.in_white(x, y)) {
            controller_.set_human_color(WHITE);
            view_.update_options(controller_);
            spdlog::info("Player selected White");
        } else if (view_.in_black(x, y)) {
            controller_.set_human_color(BLACK);
            view_.update_options(controller_);
            spdlog::info("Player selected Black");
        } else if (view_.in_tough(x, y)) {
            controller_.set_engine(Tough);
            view_.update_options(controller_);
            spdlog::info("Player selected Tough (0)");
        } else if (view_.in_hard(x, y)) {
            controller_.set_engine(Hard);
            view_.update_options(controller_);
            spdlog::info("Player selected Hard (1)");
        } else if (view_.in_impossible(x, y)) {
            controller_.set_engine(Impossible);
            view_.update_options(controller_);
            spdlog::info("Player selected Impossible (2)");
        } else if (controller_.human_color() != BOTH && controller_.engine() != NO_ENGINE && view_.in_selected(x, y)) {
            spdlog::info("Game Setup Complete... Starting Game.");
            context_->music_player_.play_startup_music(controller_.human_color());
            context_->transition_to(new PlayScreen(controller_.human_color(), controller_.engine()));
        }
        break;
    }
    default:
        spdlog::trace("Unhandled case in SplashScreen...");
    }
}


void BattleSetupScreen::draw(sf::RenderWindow& window) {
    view_.draw(window);
}
