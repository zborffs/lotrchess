#ifndef LOTRCHESS_ENGINE_INTERFACE_HPP
#define LOTRCHESS_ENGINE_INTERFACE_HPP

/// third party defines
#include <spdlog/spdlog.h>

/// stl defines
#include <chrono>
#include <thread>
#include <iostream>

/// lotrchess defines
#include "defines.hpp"
#include "extern.hpp"
#include "subprocess.hpp"

namespace sp = subprocess;

class EngineInterface {
private:
    /// engine enum to path map
    std::unordered_map<Engine, std::string> ENGINE_PATH_MAP{{ // should be const but making it const messes things up
        {CPW, "../../res/engine/senpai"},
        {Senpai, "../../res/engine/senpai"},
        {Prometheus, "../../res/engine/senpai"},
        {Stockfish, "../../res/engine/senpai"}
    }};

    Engine engine_;
    std::thread engine_thread_;

    /// engine data
    std::string best_move_;

    /// whether or not to quit (quit signal)
    bool quit_{false};
    std::mutex mu_quit_;

    bool check_quit() {
        std::lock_guard<std::mutex> guard(mu_quit_);
        return quit_;
    }

    void engine_comms_loop() {
        auto thread_id_hash = std::hash<std::thread::id>()(std::this_thread::get_id());

        /// setup interprocess communication with engine over pipes
        auto path_to_engine = ENGINE_PATH_MAP[engine_]; // get the path to the engine executable
        auto p = sp::Popen({path_to_engine}, sp::input{sp::PIPE}, sp::output{sp::PIPE}); // create a subprocess from the executable

        /// initiate UCI protocol
        auto msg = "uci"; // thing we want to send
        p.send(msg, strlen(msg)); // send it
        auto res = p.communicate(); // grab the result

        spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, msg);
        spdlog::info("Thread {} received: \"{}\"", thread_id_hash, res.first.buf.data());
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
