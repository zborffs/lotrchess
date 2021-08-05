#include "battle_setup_controller.hpp"

BattleSetupController::BattleSetupController() : human_color_(WHITE), engine_(Prometheus) {
    spdlog::info("Created BattleSetupController...");
}

BattleSetupController::~BattleSetupController() {
    spdlog::info("Destroyed BattleSetupController...");
}
