#ifndef LOTRCHESS_BATTLE_DIALOG_HPP
#define LOTRCHESS_BATTLE_DIALOG_HPP

/// third party includes
#include <spdlog/spdlog.h>

/// project includes
#include "defines.hpp"

/**
 * Controller (in MVC context) of the Battle Setup Screen
 * - responsible for:
 *   1. managing state information when on the Battle Setup Screen
 */
class BattleSetupController {
    Color human_color_; // the color the player has selected
    Engine engine_; // the engine the player selected to play against

public:
    explicit BattleSetupController();
    ~BattleSetupController();

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

    void set_human_color(Color color) {
        human_color_ = color;
    }

    void set_engine(Engine engine) {
        engine_ = engine;
    }
};

#endif // LOTRCHESS_BATTLE_DIALOG_HPP
