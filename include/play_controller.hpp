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
#include "board.hpp"

/**
 * keeps track of the current chess position and the graphics (model of the PlayView)
 * - where pieces are
 * - whose turn it is
 */
class PlayController {
private:
    /// chess game-state member variables
    Board board_;
    std::mutex mu_board_;

    /// non-chess related game-state member variables
    bool as_white_;
    Color player_color_;
    std::vector<Square_t> highlights_;

    /// move generation member variables
    std::thread move_gen_thread_;

    /// engine communication
    EngineInterface engine_io_;

    /// private member functions
    void gen_moves();

public:
    explicit PlayController(Color player_color, Engine engine, const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    ~PlayController() {
        if (move_gen_thread_.joinable()) {
            move_gen_thread_.join();
        }
        spdlog::info("Destroyed PlayController...");
    }

    [[nodiscard]] inline char piece_at(const Square sq) const {
        auto piece_type = board_.piece_type(sq);
        char ret;
        switch (piece_type) {
        case W_PAWN: ret = 'P'; break;
        case B_PAWN: ret = 'p'; break;
        case W_ROOK: ret = 'R'; break;
        case B_ROOK: ret = 'r'; break;
        case W_KNIGHT: ret = 'N'; break;
        case B_KNIGHT: ret = 'n'; break;
        case W_BISHOP: ret = 'B'; break;
        case B_BISHOP: ret = 'b'; break;
        case W_QUEEN: ret = 'Q'; break;
        case B_QUEEN: ret = 'q'; break;
        case W_KING: ret = 'K'; break;
        case B_KING: ret = 'k'; break;
        default: ret = ' ';
        }
        return ret;
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
        // board.make_move(); //?
//        side2move_ = side2move_ == WHITE ? side2move_ = BLACK : side2move_ = WHITE;
    }

    void add_highlight(Square_t sq);

    bool valid_highlight(Square_t sq);

    void clear_highlights();

    friend std::ostream& operator<<(std::ostream& os, PlayController& controller) {
        os << controller.board_ << std::endl;
        os << "Highlighted Squares: [ ";
        for (const auto& sq : controller.highlights_) {
            os << static_cast<int>(sq) << " ";
        }
        return os << "]";
    }
};

#endif // LOTRCHESS_PLAY_CONTROLLER_HPP
