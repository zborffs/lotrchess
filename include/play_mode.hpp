#ifndef LOTRCHESS_PLAY_MODE_HPP
#define LOTRCHESS_PLAY_MODE_HPP

#include "lotrchess.hpp"
#include <unordered_map>
#include <string>
#include <array>
#include "board_view.hpp"
#include "board.hpp"
#include "splash_mode.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class PlayMode : public Mode {
    const sf::Vector2f board_offset{50., 75.};
    const std::string board_texture_path{"../../res/img/ChessBoard.png"};
    const std::string one_ring_texture_path{"../../res/img/one_ring.png"};
    const std::unordered_map<char, std::string> texture_map_paths{{
        {'P', "../../res/img/Light_Pawn.png"},
        {'R', "../../res/img/Light_Rook.png"},
        {'N', "../../res/img/Light_Knight.png"},
        {'B', "../../res/img/Light_Bishop.png"},
        {'K', "../../res/img/Light_King.png"},
        {'Q', "../../res/img/Light_Queen.png"},
        {'p', "../../res/img/Dark_Pawn.png"},
        {'r', "../../res/img/Dark_Rook.png"},
        {'n', "../../res/img/Dark_Knight.png"},
        {'b', "../../res/img/Dark_Bishop.png"},
        {'k', "../../res/img/Dark_King.png"},
        {'q', "../../res/img/Dark_Queen.png"}
    }};
    Board board_{};
    BoardView board_view_{board_, board_offset, board_texture_path, one_ring_texture_path, texture_map_paths};

public:
    explicit PlayMode() {
        spdlog::info("Created PlayMode");
    }

    ~PlayMode() {
        spdlog::info("Destroyed PlayMode");
    }

    void process_event(sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LOTRCHESS_PLAY_MODE_HPP
