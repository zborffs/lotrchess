#ifndef LOTRCHESS_PLAY_SCREEN_HPP
#define LOTRCHESS_PLAY_SCREEN_HPP

#include <unordered_map>
#include <string>
#include <array>

#include <SFML/Graphics/RenderWindow.hpp>

/// project includes
#include "extern.hpp"
#include "lotrchess.hpp"
#include "play_controller.hpp"
#include "play_view.hpp"
#include "splash_screen.hpp"

class PlayScreen : public Screen {
    PlayController controller_;
    PlayView board_view_; //{controller_, sf::Vector2f{50., 75.}, BOARD_TEXTURE_PATH, ONE_RING_TEXTURE_PATH, PIECE_TEXTURE_PATH_MAP};

public:
  explicit PlayScreen(Color player_color, Engine engine) : controller_(player_color, engine), board_view_(controller_, sf::Vector2f{50., 75.}, BOARD_TEXTURE_PATH, ONE_RING_TEXTURE_PATH, PIECE_TEXTURE_PATH_MAP) {
        spdlog::info("Created PlayScreen...");
    }

    ~PlayScreen() {
        spdlog::info("Destroyed PlayScreen...");
    }

    void process_event(sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LOTRCHESS_PLAY_SCREEN_HPP
