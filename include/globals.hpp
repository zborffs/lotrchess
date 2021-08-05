#ifndef LOTRCHESS_GLOBALS_HPP
#define LOTRCHESS_GLOBALS_HPP

#include "defines.hpp"
#include "extern.hpp"

#include <string>
#include <array>

/// initialize constant variables
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 1080;
const unsigned int FPS_LIMIT = 30;
const float SCALE_FACTOR = 2.0;
const float FONT_SCALE_FACTOR = 1.0;
const std::string WINDOW_TITLE{"Lord of the Rings: Chess"};

const std::array<std::string, 4> MUSIC_PATHS({
    "../../res/music/white_startup_music.wav",
    "../../res/music/white_victory_music.wav",
    "../../res/music/black_startup_music.wav",
    "../../res/music/black_victory_music.wav",
});

#endif // LOTRCHESS_GLOBALS_HPP
