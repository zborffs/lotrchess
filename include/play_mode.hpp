#ifndef LOTRCHESS_PLAY_MODE_HPP
#define LOTRCHESS_PLAY_MODE_HPP

#include <unordered_map>
#include <string>
#include <array>

#include <SFML/Graphics/RenderWindow.hpp>

/// project includes
#include "board.hpp"
#include "board_view.hpp"
#include "extern.hpp"
#include "lotrchess.hpp"
#include "splash_screen.hpp"

class PlayScreen : public Screen {
    const sf::Vector2f board_offset{50., 75.};
    Board board_{};
    BoardView board_view_{board_, board_offset, BOARD_TEXTURE_PATH, ONE_RING_TEXTURE_PATH, PIECE_TEXTURE_PATH_MAP};

public:
    explicit PlayScreen() {
        spdlog::info("Created PlayScreen");
    }

    ~PlayScreen() {
        spdlog::info("Destroyed PlayScreen");
    }

    void process_event(sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LOTRCHESS_PLAY_MODE_HPP
