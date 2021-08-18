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
            spdlog::info("Choosing White");
        } else if (view_.in_black(x, y)) {
            controller_.set_human_color(BLACK);
            view_.update_options(controller_);
            spdlog::info("Choosing Black");
        } else if (controller_.human_color() != BOTH && view_.in_selected(x, y)) {
            spdlog::info("Game Setup Complete... Starting Game.");
            context_->music_player_.play_startup_music(controller_.human_color());
            context_->transition_to(new PlayScreen(controller_.human_color(), Prometheus));
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
