#ifndef LOTRCHESS_PLAY_MODE_HPP
#define LOTRCHESS_PLAY_MODE_HPP


#include <unordered_map>
#include <string>
#include <array>

#include <SFML/Graphics/RenderWindow.hpp>

/// project includes
#include "extern.hpp"
#include "lotrchess.hpp"
#include "board_view.hpp"
#include "board.hpp"
#include "splash_mode.hpp"

class PlayMode : public Mode {
    const sf::Vector2f board_offset{50., 75.};
    Board board_{};
    BoardView board_view_{board_, board_offset, BOARD_TEXTURE_PATH, ONE_RING_TEXTURE_PATH, PIECE_TEXTURE_PATH_MAP};

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
