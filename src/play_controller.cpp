#include "play_controller.hpp"

PlayController::PlayController(Color player_color, Engine engine, const std::string &fen) : board_(fen), as_white_(
        player_color != BLACK), player_color_(player_color), highlights_({}), engine_io_(engine) {
    /// move generation thread initialization
    if (board_.side_2_move() == player_color_) {
        std::thread t(&PlayController::gen_moves, this);
        move_gen_thread_ = std::move(t);
    } else {
        engine_io_.update_position(board_);
        engine_io_.calculate_best_move();
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

    for (const auto &m : legal_moves_) {
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

    for (auto itr = legal_moves_.begin(); itr != legal_moves_.end();) {
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

bool PlayController::check_result() {
//    auto thread_id_hash = std::hash<std::thread::id>()(std::this_thread::get_id());
//    spdlog::info("Thread {}: checking result!", thread_id_hash);
    std::string move = engine_io_.best_move();

    if (move.empty()) {
        return false;
    }

    if (move.ends_with(' ')) {
        move = move.substr(0, 4);
    }

    Square_t from_sq, to_sq;
    MoveFlag flags = QUIET_MOVE;
    PieceType_t ptm, ptc;

    /// All moves must have a length: 3 < length < 6
    if (move.size() < 4 || move.size() > 5) {
        spdlog::error("Argument error move.size() invalid: {}", move.size());
        return false;
    } else {
        /// Transform input "from square" string into Square_t primitive
        File_t file = static_cast<File_t>(move[0] - 'a');
        Rank_t rank = static_cast<Rank_t>(move[1] - '1');
        from_sq = file + 8 * rank; // multiplication will be optimized into << 3
        assert(from_sq >= A1 && from_sq <= H8); // in debug-mode, assert to crash program
        if (from_sq < A1 || from_sq > H8) {     // in release-mode, catch return a dummy ChessMove obj and log it.
            spdlog::error("Argument error from_sq out of bounds: {}", from_sq);
            return false;
        }

        /// Transform input "to square" string into Square_t primitive
        file = static_cast<File_t>(move[2] - 'a');
        rank = static_cast<Rank_t>(move[3] - '1');
        to_sq = file + 8 * rank;

        assert(to_sq >= A1 && to_sq <= H8); // in debug-mode, assert to crash program
        if (to_sq < A1 || to_sq > H8) {     // in release-mode, catch return a dummy ChessMove obj and log it.
            spdlog::error("Argument error to_sq out of bounds: {}", from_sq);
            return false;
        }

        /// Use "from squre" Square_t to figure out the piece that was moved
        ptm = board_.piece_type(from_sq);

        assert(ptm != PieceType::NO_PIECE); // in debug-mode, assert to crash program
        if (ptm == PieceType::NO_PIECE) {   // in release-mode, catch return a dummy ChessMove obj and log it.
            spdlog::error("Argument error piece @ from_sq is NO_PIECE.");
            return false;
        }

        /// User "to square" Square_t to figure out the piece that was captured
        ptc = board_.piece_type(to_sq);
        assert(ptc == PieceType::NO_PIECE ||
               ptc % 2 != ptm % 2); // make sure there are no captures, or the piece colors aren't the
        if (ptc != PieceType::NO_PIECE &&
            ptc % 2 == ptm % 2) { /// make sure captured piece is not the same color as moved piece
            spdlog::error("Argument error ptc is same color as ptm.");
            return false;
        }

        /// Set the move flags: if there was a captured piece, then it's a captured move
        if (ptc != PieceType::NO_PIECE) {
            flags = CAPTURE_MOVE;

            /// If it was an en-passant move, then set the en passant flag
        } else if (((ptm == PieceType::W_PAWN && (to_sq - from_sq == 9 || to_sq - from_sq == 7)) ||
                    (ptm == PieceType::B_PAWN && (from_sq - to_sq == 9 || from_sq - to_sq == 7))) &&
                   to_sq == board_.ep_sq()) {
            flags = ENPASSANT;

            /// if the moved piece was a pawn and it moved 2 squares, then set the double-push-pawn flag
        } else if ((ptm == PieceType::W_PAWN && to_sq - from_sq == 16 && from_sq / 8 == Rank::RANK2) ||
                   (ptm == PieceType::B_PAWN && from_sq - to_sq == 16 && from_sq / 8 == Rank::RANK7)) {
            flags = DOUBLE_PUSH_PAWN;

            /// If it was a castle, then set the castle flag
        } else if (ptm == PieceType::W_KING || ptm == PieceType::B_KING) {
            if ((from_sq == E1 && to_sq == G1) || (from_sq == E8 && to_sq == G8)) {
                flags = KING_CASTLE;
            } else if ((from_sq == E1 && to_sq == C1) || (from_sq == E8 && to_sq == C8)) {
                flags = QUEEN_CASTLE;
            }
        }

        /// If there was a fifth character, then that indicates a promotion, and we need to figure that out
        if (move.size() == 5) {
            if ((ptm == W_PAWN && to_sq / 8 == Rank::RANK8) || (ptm == B_PAWN && to_sq / 8 == Rank::RANK1)) {
                switch (move[4]) {
                    case 'N':
                    case 'n':
                        flags = static_cast<MoveFlag>(KNIGHT_PROMO + flags);
                        break;
                    case 'B':
                    case 'b':
                        flags = static_cast<MoveFlag>(BISHOP_PROMO + flags);
                        break;
                    case 'R':
                    case 'r':
                        flags = static_cast<MoveFlag>(ROOK_PROMO + flags);
                        break;
                    case 'Q':
                    case 'q':
                        flags = static_cast<MoveFlag>(QUEEN_PROMO + flags);
                        break;
                }
            } else {
                spdlog::error("Argument error size == 5, but from/to_sq invalid for promotion.");
                return false;
            }
        }
    }

    ChessMove chess_move(from_sq, to_sq, flags, ptm, ptc);
    spdlog::info("Generated Move: {}", chess_move.to_string());
    engine_io_.reset_best_move();

    std::lock_guard<std::mutex> guard(mu_board_);
    board_.make_move(chess_move);
    std::cout << board_ << std::endl;

    engine_io_.update_position(board_);

    if (board_.side_2_move() == player_color_) {
        if (move_gen_thread_.joinable()) {
            move_gen_thread_.join(); // the thread must be joined here!
        }
        std::thread t(&PlayController::gen_moves, this);
        move_gen_thread_ = std::move(t);
    } else {
        engine_io_.calculate_best_move();
    }

    return true;
}
