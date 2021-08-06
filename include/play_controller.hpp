#ifndef LOTRCHESS_PLAY_CONTROLLER_HPP
#define LOTRCHESS_PLAY_CONTROLLER_HPP

/// third party includes
#include <spdlog/spdlog.h>

/// stl defines
#include <array>
#include <string>
#include <iostream>
#include <thread>

/// lotrchess defines
#include "defines.hpp"
#include "string_manip.hpp"
#include "engine_interface.hpp"

/**
 * keeps track of the current chess position and the graphics (model of the PlayView)
 * - where pieces are
 * - whose turn it is
 */
class PlayController {
private:
    /// game state member variables
    std::array<std::array<char, 8>, 8> piece_locations_{}; // a1 = [0][0], a8 = [0][7]
    Color side2move_;
    bool as_white_;
    Color player_color_;
    std::vector<Square_t> highlights_;

    /// move generation member variables
    std::thread move_gen_thread_;
    std::unordered_map<Square_t, std::vector<Square_t>> legal_move_map_;
    std::mutex mu_legal_move_map_;
    bool stop_gen_move_{false};
    std::mutex mu_stop_gen_move_;
    void gen_moves();
    bool check_stop_gen_move() {
        std::lock_guard<std::mutex> guard(mu_stop_gen_move_);
        return stop_gen_move_;
    }
    void stop_move_gen() {
        std::lock_guard<std::mutex> guard(mu_stop_gen_move_);
        stop_gen_move_ = true;
    }

    /// engine communication
    EngineInterface engine_io_;

public:
    explicit PlayController(Color player_color, Engine engine, const std::string& fen = "RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr w");

    ~PlayController() {
        stop_move_gen();
        if (move_gen_thread_.joinable()) {
            move_gen_thread_.join();
        }
        spdlog::info("Destroying PlayController...");
    }

    [[nodiscard]] inline char piece_at(const Square sq) const {
        return piece_locations_[sq / 8][sq % 8];
    }

    [[nodiscard]] inline Color side_2_move() const noexcept {
        return side2move_;
    }

    [[nodiscard]] inline bool as_white() noexcept {
        return as_white_;
    }

    [[nodiscard]] inline bool as_white() const noexcept {
        return as_white_;
    }

    [[nodiscard]] inline bool is_highlighted(const Square_t sq) const noexcept {
        return std::find(highlights_.begin(), highlights_.end(), sq) != highlights_.end();
    }

    void rotate_board() {
        as_white_ = !as_white_;
    }

    void switch_sides() {
        side2move_ = side2move_ == WHITE ? side2move_ = BLACK : side2move_ = WHITE;
//        std::thread t(&PlayController::gen_moves, this);
//        move_gen_thread_ = std::move(t);
    }

    void add_highlight(Square_t sq);

    bool valid_highlight(Square_t sq);

    void clear_highlights();

    friend std::ostream& operator<<(std::ostream& os, const PlayController & board) {
        for (const auto& row : board.piece_locations_) {
            for (const auto& piece : row) {
                os << piece << "-";
            }
            os << std::endl;
        }

        if (board.side2move_ == Color::WHITE) {
            os << "White's Move\n";
        } else {
            os << "Black's Move\n";
        }

        for (const auto& sq : board.highlights_) {
            os << static_cast<int>(sq) << " ";
        }

        return os;
    }
};

#endif // LOTRCHESS_PLAY_CONTROLLER_HPP
