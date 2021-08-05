#ifndef LOTRCHESS_BATTLE_DIALOG_HPP
#define LOTRCHESS_BATTLE_DIALOG_HPP

#include "defines.hpp"

class BattleSetupDialog {
    Color human_color_;
    Engine engine_;

public:
    explicit BattleSetupDialog();

    [[nodiscard]] inline Color human_color() noexcept {
        return human_color_;
    }

    [[nodiscard]] inline Color human_color() const noexcept {
        return human_color_;
    }

    [[nodiscard]] inline Engine general() noexcept {
        return engine_;
    }

    [[nodiscard]] inline Engine general() const noexcept {
        return engine_;
    }

    void toggle_human_color() {
        human_color_ = human_color_ == WHITE ? BLACK : WHITE;
    }

    void set_engine(Engine engine) {
        engine_ = engine;
    }
};

#endif // LOTRCHESS_BATTLE_DIALOG_HPP
