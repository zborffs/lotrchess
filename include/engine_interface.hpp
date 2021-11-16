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
#include "chess_move.hpp"

namespace bp = boost::process;

/**
 * upon the creation of the PlayController, the EngineInterface is also created, and the engine_comms_loop is entered by
 * the EngineInterface thread. In the engine interface thread, the engine process is created, and the UCI engine is
 * initialized to UCI mode and verfied to be ready. Then the thread sleeps until it is being asked the best move.
 */
class EngineInterface {
private:
    /// engine enum to path map
    std::unordered_map<Engine, std::string> ENGINE_PATH_MAP{{ // should be const but making it const messes things up
        {Senpai, "../../res/engine/prometheus"},
        {Prometheus, "../../res/engine/prometheus"},
        {Stockfish, "../../res/engine/prometheus"}
    }};

    Engine engine_;
    std::thread engine_thread_;
    std::string uci_position_command_{"position startpos"};
    std::mutex mu_uci_position_command_;

    /// sleep
    bool sleep_{true};
    std::mutex mu_sleep_;


    /// engine data
    std::string best_move_;
    std::mutex mu_best_move_;

    /// whether or not to quit (quit signal)
    bool quit_{false};
    std::mutex mu_quit_;

    bool check_quit() {
        std::lock_guard<std::mutex> guard(mu_quit_);
        return quit_;
    }

    bool check_sleep() {
        std::lock_guard<std::mutex> guard(mu_sleep_);
        return sleep_;
    }


    void engine_comms_loop() {
        /// setup interprocess communication with engine over pipes
        auto path_to_engine = ENGINE_PATH_MAP[engine_]; // get the path to the engine executable

        bp::ipstream out_pipe_stream;
        bp::opstream in_pipe_stream;
        bp::child c(path_to_engine, bp::std_out > out_pipe_stream, bp::std_in < in_pipe_stream);

        send_uci_command(c, out_pipe_stream, in_pipe_stream);
        send_isready_command(c, out_pipe_stream, in_pipe_stream);

        while (!check_quit()) {
            while (!check_quit() && check_sleep()) {
                // while the engine hasn't been requested to sleep or to quit, then sleep
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }

            if (check_quit()) {
                break;
            }

            send_position_command(in_pipe_stream);
            send_go_command(c, out_pipe_stream, in_pipe_stream);

            std::lock_guard<std::mutex> guard(mu_sleep_);
            sleep_ = true;
        }


        // auto move = send_go_command(c, out_pipe_stream, in_pipe_stream);
        // set_best_move(move); // move_ = transform_string_to_chessmove(move);
        // send_position_command(position); // position startpos moves e2e4 ... (from board in other class);


        send_quit_command(c, out_pipe_stream, in_pipe_stream);

        spdlog::info("Terminating process...");
    }

    void send_uci_command(bp::child& c, bp::ipstream& out_pipe_stream, bp::opstream& in_pipe_stream) {
        auto thread_id_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
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
    }

    void send_isready_command(bp::child& c, bp::ipstream& out_pipe_stream, bp::opstream& in_pipe_stream) {
        auto thread_id_hash = std::hash<std::thread::id>()(std::this_thread::get_id());

        auto msg = "isready";
        in_pipe_stream << msg << std::endl;
        spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, msg);
        std::string line;

        // setup uci mode
        while (c.running() && !check_quit()) {
            std::getline(out_pipe_stream, line);
            spdlog::info("Thread {} received: \"{}\"", thread_id_hash, line);
            if (line.empty() || line == "readyok") {
                break;
            }
        }
    }

    void send_quit_command(bp::child& c, bp::ipstream& out_pipe_stream, bp::opstream& in_pipe_stream) {
        auto thread_id_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
        in_pipe_stream << "quit" << std::endl;
        spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, "quit");
        std::string line;
        while (c.running() && !check_quit() && !line.empty()) {
            std::getline(out_pipe_stream, line);
            spdlog::info("Thread {} received: \"{}\"", thread_id_hash, line);
        }
        c.wait();
    }

    void send_position_command(bp::opstream& in_pipe_stream) {
        std::lock_guard<std::mutex> guard(mu_uci_position_command_);
        auto thread_id_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
        in_pipe_stream << uci_position_command_ << std::endl;
        spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, uci_position_command_);
    }

    void send_go_command(bp::child& c, bp::ipstream& out_pipe_stream, bp::opstream& in_pipe_stream) {
        auto thread_id_hash = std::hash<std::thread::id>()(std::this_thread::get_id());

        switch(engine_) {
            case Senpai:
                in_pipe_stream << "go depth 3" << std::endl; // arbitrary
                spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, "go depth 1");
                break;
            case Prometheus:
                in_pipe_stream << "go movetime 5000" << std::endl; // arbitrary
                spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, "go movetime 5000");
                break;
            case Stockfish:
                in_pipe_stream << "go movetime 7500" << std::endl; // arbitrary
                spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, "go movetime 3000");
                break;
            case NO_ENGINE:
                in_pipe_stream << "go movetime 5000" << std::endl; // arbitrary
                spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, "go movetime 5000");
                break;
        }
//        in_pipe_stream << "go movetime 5000" << std::endl; // arbitrary
//        spdlog::info("Thread {} sent: \"{}\"", thread_id_hash, "go movetime 2500");
        std::string line;
        while (c.running() && !check_quit()) {
            std::getline(out_pipe_stream, line);
            spdlog::info("Thread {} received: \"{}\"", thread_id_hash, line);
            if (line.empty() || (line.size() > 8 && line.substr(0, 8) == "bestmove")) {
                break;
            }
        }

        std::lock_guard<std::mutex> guard(mu_best_move_);
        if (line.size() >= 9 + 5) {
            best_move_ = line.substr(9, 5); // allows to capture to-from squares and the promotion piece
        } else {
            best_move_ = line.substr(9, 4); // just assume we have room to extract the to-from squares
        }
        spdlog::info("Thread {} best move: \"{}\"", thread_id_hash, best_move_);
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
        std::lock_guard<std::mutex> guard(mu_best_move_);
        return best_move_;
    }

    [[nodiscard]] inline Engine engine() noexcept {
        return engine_;
    }

    [[nodiscard]] inline Engine engine() const noexcept {
        return engine_;
    }

    void update_position(Board& board) {
        auto thread_id_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
        std::lock_guard<std::mutex> guard(mu_uci_position_command_);
        std::string uci_pos_str = board.uci_pos_str();
        uci_position_command_ = "position " + uci_pos_str;
        spdlog::info("Thread {} prepping UCI command: {}", thread_id_hash, uci_position_command_);
    }

    void quit() {
        std::lock_guard<std::mutex> guard(mu_quit_);
        quit_ = true;
    }

    void calculate_best_move() {
        std::lock_guard<std::mutex> guard(mu_sleep_);
        sleep_ = false;
        spdlog::info("sleep_ is false");
    }

    void reset_best_move() {
        std::lock_guard<std::mutex> guard(mu_best_move_);
        best_move_ = "";
        spdlog::info("best_move reset");
    }
};

#endif // LOTRCHESS_ENGINE_INTERFACE_HPP
