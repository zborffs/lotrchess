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
#include "move_gen.hpp"

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
    bool piece_selected_{false}; // whether the player has clicked on a piece on the board yet!
    ResultFlag result_{NO_RESULT};

    /// non-chess related game-state member variables
    bool as_white_;
    Color player_color_;
    std::vector<Square_t> highlights_;

    /// move generation member variables
    std::thread move_gen_thread_;
    std::vector<ChessMove> legal_moves_{};
    std::mutex mu_legal_moves_;

    /// engine communication
    EngineInterface engine_io_;

    /// private member functions
    void gen_moves();

public:
    explicit PlayController(Color player_color, Engine engine, const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    ~PlayController() {
//        engine_io_.quit(); -> doesn't need to be called, since it's in the destructor
        if (move_gen_thread_.joinable()) {
            move_gen_thread_.join();
        }
        spdlog::info("Destroyed PlayController...");
    }

    [[nodiscard]] inline char piece_at(const Square sq) {
        std::lock_guard<std::mutex> move_guard(mu_legal_moves_);

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

    [[nodiscard]] inline ResultFlag result_flag() noexcept {
        return result_;
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

    [[nodiscard]] inline bool piece_selected() noexcept {
        return piece_selected_;
    }

    [[nodiscard]] inline bool piece_selected() const noexcept {
        return piece_selected_;
    }

    bool move_is_legal(Square_t to_sq) {
        if (highlights_.empty() || !piece_selected_ || legal_moves_.empty()) {
            return false;
        }

        Square_t from_sq = highlights_.back();
        for (const auto& m : legal_moves_) {
            if (m.from_sq == from_sq && m.to_sq == to_sq) {
                return true;
            }
        }
        return false;
    }

    void make_move(Square_t to_sq) {
        std::lock_guard<std::mutex> guard(mu_board_);
        std::lock_guard<std::mutex> move_guard(mu_legal_moves_);

        Square_t from_sq = highlights_.back();
        for (auto& m : legal_moves_) {
            if (m.from_sq == from_sq && m.to_sq == to_sq) {
                board_.make_move(m);
                player_color_ = player_color_ == WHITE ? BLACK : WHITE; // remove this line at some point!
                std::cout << board_ << std::endl;
                break;
            }
        }

        if (board_.side_2_move() == player_color_) {
            if (move_gen_thread_.joinable()) {
                move_gen_thread_.join(); // the thread must be joined here!
            }
            std::thread t(&PlayController::gen_moves, this);
            move_gen_thread_ = std::move(t);
        }
    }

    void rotate_board() {
        as_white_ = !as_white_;
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
