#include "play_controller.hpp"

PlayController::PlayController(Color player_color, Engine engine, const std::string& fen) : as_white_(player_color != BLACK), player_color_(player_color), highlights_({}), move_gen_thread_(&PlayController::gen_moves, this), engine_io_(engine) {
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
    side2move_ = fen_commands[1][0] == 'w' ? Color::WHITE : Color::BLACK;

    spdlog::info("Creating PlayController...");
}

/**
 *
 * @param sq
 */
void PlayController::add_highlight(Square_t sq) {
    highlights_.push_back(sq);
}

/**
 * returns true if the square being selected is "valid"
 * - must be the player's turn to move [done]
 * - must be the player's pieces [done]
 * - must be a player's piece that has a move
 * - must not already be highlighted [done]
 * @param sq the square being checked for highlighting
 * @return true if a valid square to highlight, false otherwise
 */
bool PlayController::valid_highlight(Square_t sq) {
    if (player_color_ != side2move_ || std::find(highlights_.begin(), highlights_.end(), sq) != highlights_.end()) {
        // if it's not the players move or the square is already in the list, return false
        return false;
    }

    char piece = piece_locations_[sq / 8][sq % 8];
    if (piece >= 'a' && piece <= 'z' && side2move_ == BLACK) {
        // if the piece is black and it's black's turn (player==black is implicit from previous check), then return true
        return true;
    } else if (piece >= 'A' && piece <= 'Z' && side2move_ == WHITE) {
        // if the piece is white and it's white's turn (player==white is implicit from previous check), then return true
        return true;
    }

    return false;
}

/**
 * clear the highlight vector (don't highlight anything anymore)
 */
void PlayController::clear_highlights() {
    highlights_.clear();
}

/**
 * generate moves
 */
void PlayController::gen_moves() {
    auto hash = std::hash<std::thread::id>()(std::this_thread::get_id());
    for (int i = 0; i < 2 && !check_stop_gen_move(); ++i) {
        spdlog::info("gen_moves() - thread_id: {}, i: {}", hash, i); // std::this_thread::get_id()
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
