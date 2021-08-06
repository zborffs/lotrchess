#ifndef LOTRCHESS_ENGINE_INTERFACE_HPP
#define LOTRCHESS_ENGINE_INTERFACE_HPP

/// third party defines
#include <spdlog/spdlog.h>

/// stl defines
#include <chrono>
#include <thread>

/// lotrchess defines
#include "defines.hpp"

class EngineInterface {
private:
    Engine engine_;
    std::thread engine_thread_;
    std::string best_move_;
    bool quit_{false};
    std::mutex mu_quit_;

    bool check_quit() {
        std::lock_guard<std::mutex> guard(mu_quit_);
        return quit_;
    }

    void engine_comms_loop() {
//        auto hash = std::hash<std::thread::id>()(std::this_thread::get_id());
//        for (int i = 0; i < 2 && !check_quit(); ++i) {
//            spdlog::info("engine_comms_loop() - thread_id: {}, i: {}", hash, i);
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//        }
    }

    void quit() {
        std::lock_guard<std::mutex> guard(mu_quit_);
        quit_ = true;
    }

public:
    explicit EngineInterface(Engine engine) : engine_(engine), engine_thread_(&EngineInterface::engine_comms_loop, this) {
        spdlog::info("Created EngineInterface...");
    }

    ~EngineInterface() {
        quit();
        if (engine_thread_.joinable()) {
            engine_thread_.join();
        }
        spdlog::info("Destroyed EngineInterface...");
    }



    [[nodiscard]] inline std::string best_move() noexcept {
        return best_move_;
    }

    [[nodiscard]] inline std::string best_move() const noexcept {
        return best_move_;
    }

    [[nodiscard]] inline Engine engine() noexcept {
        return engine_;
    }

    [[nodiscard]] inline Engine engine() const noexcept {
        return engine_;
    }
};

#endif // LOTRCHESS_ENGINE_INTERFACE_HPP
