#ifndef LOTRCHESS_EXTERN_HPP
#define LOTRCHESS_EXTERN_HPP

/// third party includes
#include <SFML/Graphics/Color.hpp>

/// stl includes
#include <string>
#include <array>
#include <unordered_map>

/// project includes
#include "defines.hpp"


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

extern const uint8_t CASTLE_CHECK[64];

extern const PositionKey POSITION_KEYS[12][64];
extern const PositionKey SIDE_2_MOVE_KEYS[2];
extern const PositionKey CASTLE_PERMISSION_KEYS[16];
extern const PositionKey EN_PASSANT_KEYS[17];

extern const Bitboard ANTI_DIAGONAL_MASK[15];
extern const Bitboard DIAGONAL_MASK[15];
extern const Bitboard RANK_MASK[8];
extern const Bitboard FILE_MASK[8];

extern const Bitboard KING_ATTACK_ARRAY[64];
extern const Bitboard KNIGHT_ATTACK_ARRAY[64];

#endif // LOTRCHESS_EXTERN_HPP
