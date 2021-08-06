#include "play_controller.hpp"

PlayController::PlayController(Color player_color, Engine engine, const std::string& fen) : board_(fen), as_white_(player_color != BLACK), player_color_(player_color), highlights_({}), engine_io_(engine) {

    /// move generation thread initialization
    if (board_.side_2_move() == player_color_) {
        std::thread t(&PlayController::gen_moves, this);
        move_gen_thread_ = std::move(t);
    }

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
    auto s2m = board_.side_2_move(); // later I will need to mutex access to board, so pull out calls thereto to single lines
    auto piece_type = board_.piece_type(sq);

    if (player_color_ != s2m || piece_type == NO_PIECE || std::find(highlights_.begin(), highlights_.end(), sq) != highlights_.end()) {
        // if it's not the players move or the square is already in the list, return false
        return false;
    }
    Color_t selected_color = (piece_type % 2);
    if (selected_color == BLACK && s2m == BLACK) {
        // if the piece is black and it's black's turn (player==black is implicit from previous check), then return true
        return true;
    } else if (selected_color == WHITE && s2m == WHITE) {
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
//    std::lock_guard<std::mutex> guard(mu_board_);
//    gen_all_moves(board_);
    spdlog::info("gen_moves() - thread_id: {}", hash); // std::this_thread::get_id()
}
