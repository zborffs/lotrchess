#ifndef LOTRCHESS_BOARD_HPP
#define LOTRCHESS_BOARD_HPP

/// lotrchess defines
#include "defines.hpp"
#include "string_manip.hpp"

/// stl defines
#include <array>
#include <string>
#include <iostream>

/**
 * keeps track of the current chess position and the graphics (model of the BoardView)
 * - where pieces are
 * - whose turn it is
 */
class Board {
private:
    std::array<std::array<char, 8>, 8> piece_locations_{}; // a1 = [0][0], a8 = [0][7]
    Color side_2_move_;
    bool as_white_ {true};
    std::vector<Square_t> highlights_;

public:
    explicit Board(const std::string& fen = "RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr w");

    [[nodiscard]] inline char piece_at(const Square sq) const {
        return piece_locations_[sq / 8][sq % 8];
    }

    [[nodiscard]] inline Color side_2_move() const noexcept {
        return side_2_move_;
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

    void switch_sides() {
        as_white_ = !as_white_;
    }

    void add_highlight(Square_t sq);

    friend std::ostream& operator<<(std::ostream& os, const Board& board) {
        for (const auto& row : board.piece_locations_) {
            for (const auto& piece : row) {
                os << piece << " ";
            }
            os << std::endl;
        }

        if (board.side_2_move_ == Color::WHITE) {
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

#endif // LOTRCHESS_BOARD_HPP
