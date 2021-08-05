#ifndef LOTRCHESS_GLOBALS_HPP
#define LOTRCHESS_GLOBALS_HPP

/// third party includes
#include <SFML/Graphics/Color.hpp>

/// project includes
#include "defines.hpp"
#include "extern.hpp"

/// stl includes
#include <string>
#include <array>
#include <unordered_map>

/// Window constants
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 1080;
const unsigned int FPS_LIMIT = 30;
const float SCALE_FACTOR = 2.0;
const float FONT_SCALE_FACTOR = 1.0;
const std::string WINDOW_TITLE{"Lord of the Rings: Chess"};

/// resource paths (relative to cmake build)
const std::array<std::string, 4> MUSIC_PATHS({
    "../../res/music/white_startup_music.wav",
    "../../res/music/white_victory_music.wav",
    "../../res/music/black_startup_music.wav",
    "../../res/music/black_victory_music.wav",
});
const std::string FONT_PATH{"../../res/font/lotr_font.ttf"};
const std::string SPLASH_BG_PATH{"../../res/img/splash_bg.jpg"};
const std::string BOARD_TEXTURE_PATH{"../../res/img/ChessBoard.png"};
const std::string ONE_RING_TEXTURE_PATH{"../../res/img/one_ring.png"};
const std::unordered_map<char, std::string> PIECE_TEXTURE_PATH_MAP{{
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

/// colors (color scheme)
const sf::Color DEFAULT_FONT_COLOR{255, 255, 255, 255}; // solid white
const sf::Color SELECTED_FONT_COLOR{255, 215,0, 255}; // LOTR gold

#endif // LOTRCHESS_GLOBALS_HPP
