#include "battle_setup_mode.hpp"
void BattleSetupMode::process_event(sf::Event& event) {
    ImGui::SFML::ProcessEvent(event);

    switch (event.type) {
    case sf::Event::Closed: {
        // if it was a close window event, then close the window
        context_->window_.close();
        break;
    }
    case sf::Event::MouseMoved: {
//        auto x = event.mouseMove.x;
//        auto y = event.mouseMove.y;
        break;
    }
    case sf::Event::MouseButtonPressed: {
        break;
    }
    default:
      spdlog::trace("Unhandled case in SplashMode...");
    }
}
void BattleSetupMode::draw(sf::RenderWindow& window) {
    dialog_view_.draw(window);
}
