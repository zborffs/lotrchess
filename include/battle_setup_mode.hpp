#ifndef LOTRCHESS_BATTLE_SETUP_MODE_HPP
#define LOTRCHESS_BATTLE_SETUP_MODE_HPP

#include "lotrchess.hpp"
#include "battle_setup_dialog.hpp"
#include "battle_setup_dialog_view.hpp"
#include "play_mode.hpp"

class BattleSetupMode : public Mode {
    BattleSetupDialog dialog_;
    BattleSetupDialogView dialog_view_;

public:
    explicit BattleSetupMode() {
        spdlog::info("Created BattleSetupMod");
    }

    ~BattleSetupMode() {
        spdlog::info("Destroyed Battle Setup Mode");
    }

    void process_event(sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LOTRCHESS_BATTLE_SETUP_MODE_HPP
