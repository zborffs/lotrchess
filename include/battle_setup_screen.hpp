#ifndef LOTRCHESS_BATTLE_SETUP_SCREEN_HPP
#define LOTRCHESS_BATTLE_SETUP_SCREEN_HPP

#include "lotrchess.hpp"
#include "battle_setup_dialog.hpp"
#include "battle_setup_dialog_view.hpp"
#include "play_mode.hpp"

class BattleSetupScreen : public Screen {
    BattleSetupDialog dialog_;
    BattleSetupDialogView dialog_view_;

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
