#include "splash_mode.hpp"


void SplashMode::process_event(sf::Event &event) {
    ImGui::SFML::ProcessEvent(event);

    switch (event.type) {
    case sf::Event::Closed: {
        // if it was a close window event, then close the window
        context_->window_.close();
        break;
    }
    case sf::Event::MouseMoved: {
        auto x = event.mouseMove.x;
        auto y = event.mouseMove.y;
        if (dialog_view_.in_campaign(x, y)) {
            if (dialog_.selection() != SplashSelector::Campaign) {
                dialog_.set_selection(SplashSelector::Campaign);
                dialog_view_.update_options(dialog_);
            }
        } else if (dialog_view_.in_battle(x, y)) {
            if (dialog_.selection() != SplashSelector::CustomBattle) {
                dialog_.set_selection(SplashSelector::CustomBattle);
                dialog_view_.update_options(dialog_);
            }
        } else if (dialog_view_.in_replay(x, y)) {
            if (dialog_.selection() != SplashSelector::Replay) {
                dialog_.set_selection(SplashSelector::Replay);
                dialog_view_.update_options(dialog_);
            }
        } else if (dialog_view_.in_quit(x, y)) {
            if (dialog_.selection() != SplashSelector::Quit) {
                dialog_.set_selection(SplashSelector::Quit);
                dialog_view_.update_options(dialog_);
            }
        } else {
            if (dialog_.selection() != SplashSelector::None) {
                dialog_.set_selection(SplashSelector::None);
                dialog_view_.update_options(dialog_);
            }
        }
        break;
    }
    case sf::Event::MouseButtonPressed: {
        spdlog::info("ESC was pressed...");
        switch(dialog_.selection()) {
        case Campaign:
            spdlog::info("Campaign Selected...");
            break;
        case CustomBattle:
            spdlog::info("Custom Battle Selected...");
//            context_->transition_to(new PlayMode());
            context_->transition_to(new BattleSetupMode());
            break;
        case Replay:
            spdlog::info("Replay Selected...");
            break;
        case Quit:
            // if asked to close, then close
            spdlog::info("Quit Selected...");
            context_->window_.close();
            break;
        case None:
            break;
        }

    }
    default:
        spdlog::trace("Unhandled case in SplashMode...");
    }
}

void SplashMode::draw(sf::RenderWindow &window) {
    dialog_view_.draw(window);
}