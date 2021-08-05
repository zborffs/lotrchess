#ifndef LOTRCHESS_BATTLE_SETUP_SCREEN_HPP
#define LOTRCHESS_BATTLE_SETUP_SCREEN_HPP

#include "battle_setup_controller.hpp"
#include "battle_setup_view.hpp"
#include "lotrchess.hpp"
#include "play_screen.hpp"

class BattleSetupScreen : public Screen {
    BattleSetupController dialog_;
    BattleSetupView dialog_view_;

public:
    explicit BattleSetupScreen() {
        spdlog::info("Created BattleSetupScreen");
    }

    ~BattleSetupScreen() {
        spdlog::info("Destroyed BattleSetupScreen");
    }

    void process_event(sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LOTRCHESS_BATTLE_SETUP_SCREEN_HPP
