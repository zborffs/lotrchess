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
    if (!highlights_.empty() && highlights_.back() == sq) {
        // if the square that we are seeking to highlight is already highlighted, then move on!
        return;
    }

    highlights_.clear(); // clear the highlights

    // TODO: ADD CHECK TO MAKE SURE THAT LEGAL_MOVES IS NON-EMPTY I.E. NO LOSING!
    for (const auto& m : legal_moves_) {
        if (m.from_sq == sq) {
            highlights_.push_back(m.to_sq);
        }
    }

    if (!highlights_.empty()) {
        highlights_.push_back(sq); // always the last one!
        piece_selected_ = true;
    } else {
        piece_selected_ = false;
    }
}

/**
 * returns true if the square being selected is "valid"
 * - must be the player's turn to move [done]
 * - must be the player's pieces [done]
 * - must not already be highlighted [done]
 * @param sq the square being checked for highlighting
 * @return true if a valid square to highlight, false otherwise
 */
bool PlayController::valid_highlight(Square_t sq) {
    std::lock_guard<std::mutex> guard(mu_board_);
    auto s2m = board_.side_2_move(); // later I will need to mutex access to board, so pull out calls thereto to single lines
    auto piece_type = board_.piece_type(sq);

    if (player_color_ != s2m || piece_type == NO_PIECE) {
        // if it's not the players move or the square has no piece on it, return false
        piece_selected_ = false;
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

    piece_selected_ = false;
    return false;
}

/**
 * clear the highlight vector (don't highlight anything anymore)
 */
void PlayController::clear_highlights() {
    piece_selected_ = false;
    highlights_.clear();
}

/**
 * generate moves
 */
void PlayController::gen_moves() {
    std::lock_guard<std::mutex> board_guard(mu_board_);
    std::lock_guard<std::mutex> move_guard(mu_legal_moves_);
    legal_moves_.clear();
    legal_moves_.reserve(128);

    gen_all_moves(board_, legal_moves_);

    for (auto itr = legal_moves_.begin(); itr != legal_moves_.end(); ) {
//        spdlog::info("Board::Key, before make: {}", board_.key());
        board_.make_move(*itr);
        if (board_.is_king_checked(!board_.side_2_move())) {
            // the king of the side that just moved is in check, so this is an illegal move. erase it
            itr = legal_moves_.erase(itr);
        } else {
            ++itr; // don't erase if legal!
        }
        board_.unmake_move();
//        spdlog::info("Board::Key, after make: {}", board_.key());
    }

    if (legal_moves_.empty()) {
        // either draw or stalemate
        if (board_.is_king_checked(board_.side_2_move())) {
            // mate
            if (board_.side_2_move() == WHITE) {
                result_ = BLACK_VICTORY;
            } else {
                result_ = WHITE_VICTORY;
            }
        } else {
            result_ = STALEMATE;
        }
    } else if (board_.is_draw()) {
        result_ = DRAW;
    }
}
