#include "board.hpp"

Board::Board(const std::string& fen) {

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