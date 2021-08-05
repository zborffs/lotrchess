#include "board.hpp"

Board::Board(const std::string& fen) : highlights_({}) {

    // assume that the fen has been passed in correctly
    std::size_t r{0};
    std::size_t c{0};

    const std::vector<std::string> fen_commands = split(fen, ' ');
    assert(fen_commands.size() == 2);

    for (const auto& ch : fen_commands[0]) {
        if  (ch >= '0' && ch <= '9') {
            auto usigned_val = static_cast<unsigned int>(ch - '0');

            for (unsigned i = 0; i < usigned_val; ++i, ++c) {
                piece_locations_[r][c] = ' ';
            }
        } else if (ch == '/') {
            ++r;
            c = 0;
        } else {
            piece_locations_[r][c] = ch;
            ++c;
        }
    }

    assert(fen_commands[1].size() == 1);

    side_2_move_ = fen_commands[1][0] == 'w' ? Color::WHITE : Color::BLACK;
}
void Board::add_highlight(Square_t sq) {
    highlights_.push_back(sq);
}

/**
 * returns true if the square being selected being highlighted is "valid"
 * - must be the player's turn to move
 * - must be the player's pieces
 * - must be a player's piece that has a move [done]
 * - must not already be highlighted [done]
 * @param sq
 * @return
 */
bool Board::valid_highlight(Square_t sq) {
    if (std::find(highlights_.begin(), highlights_.end(), sq) != highlights_.end()) {
        // if the square is already in the list, return false
        return false;
    }

    char piece = piece_locations_[sq / 8][sq % 8];
    if (piece >= 'a' && piece <= 'z' && side_2_move_ == BLACK) {
        // piece is black
        return true;
    } else if (piece >= 'A' && piece <= 'Z' && side_2_move_ == WHITE) {
        // piece is white
        return true;
    }

    return false;
}
void Board::clear_highlights() {
    highlights_.clear();
}
