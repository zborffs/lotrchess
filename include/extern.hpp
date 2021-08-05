#ifndef LOTRCHESS_EXTERN_HPP
#define LOTRCHESS_EXTERN_HPP

/// project includes
#include "defines.hpp"

/// stl includes
#include <string>
#include <array>
#include <unordered_map>

/// window constants
extern const unsigned int WINDOW_WIDTH;
extern const unsigned int WINDOW_HEIGHT;
extern const unsigned int FPS_LIMIT;
extern const float SCALE_FACTOR;
extern const float FONT_SCALE_FACTOR;
extern const std::string WINDOW_TITLE;

/// resource paths (relative to cmake)
extern const std::array<std::string, 4> MUSIC_PATHS;
extern const std::string SPLASH_BG_PATH;
extern const std::string FONT_PATH;
extern const std::string BOARD_TEXTURE_PATH;
extern const std::string ONE_RING_TEXTURE_PATH;
extern const std::unordered_map<char, std::string> PIECE_TEXTURE_PATH_MAP;

/// color constants (color scheme)
extern const sf::Color DEFAULT_FONT_COLOR;
extern const sf::Color SELECTED_FONT_COLOR;

#endif // LOTRCHESS_EXTERN_HPP
