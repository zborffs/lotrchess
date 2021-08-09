#ifndef LOTRCHESS_ENGINE_INTERFACE_HPP
#define LOTRCHESS_ENGINE_INTERFACE_HPP

/// third party defines
#include <spdlog/spdlog.h>
#include <boost/process.hpp>

/// stl defines
#include <chrono>
#include <thread>
#include <iostream>

/// lotrchess defines
#include "defines.hpp"
#include "extern.hpp"
#include "board.hpp"

namespace bp = boost::process;

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
    std::string uci_position_command_{"position startpos"};

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

        bp::ipstream out_pipe_stream;
        bp::opstream in_pipe_stream;
        bp::child c(path_to_engine, bp::std_out > out_pipe_stream, bp::std_in < in_pipe_stream);

        std::string line;

        auto msg = "uci";
        in_pipe_stream << msg << std::endl;
        spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, msg);

        // setup uci mode
        while (c.running() && !check_quit()) {
            std::getline(out_pipe_stream, line);
            spdlog::info("Thread {} received: \"{}\"", thread_id_hash, line);
            if (line.empty() || line == "uciok") {
                break;
            }

        }

        msg = "isready";
        in_pipe_stream << msg << std::endl;
        spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, msg);

        // setup uci mode
        while (c.running() && !check_quit()) {
            std::getline(out_pipe_stream, line);
            spdlog::info("Thread {} received: \"{}\"", thread_id_hash, line);
            if (line.empty() || line == "readyok") {
                break;
            }
        }

        in_pipe_stream << "quit" << std::endl;
        spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, "quit");
        c.wait();
//        c.terminate();
        spdlog::info("Terminating process...");
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

    void update_position(Board& board) {
        std::string uci_pos_str = board.uci_pos_str();
        uci_position_command_ = "position " + uci_pos_str;
    }

    void quit() {
        std::lock_guard<std::mutex> guard(mu_quit_);
        quit_ = true;
    }
};

#endif // LOTRCHESS_ENGINE_INTERFACE_HPP
