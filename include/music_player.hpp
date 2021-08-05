#ifndef LOTRCHESS_MUSIC_PLAYER_HPP
#define LOTRCHESS_MUSIC_PLAYER_HPP

#include <SFML/Audio/Music.hpp>
#include <spdlog/spdlog.h>

#include <array>
#include <string>

#include "defines.hpp"

class MusicPlayer {
private:
    std::array<sf::Music, 2> victory_music_;
    std::array<sf::Music, 2> startup_music_;

public:
    explicit MusicPlayer(const std::array<std::string, 4>& music_paths);

    void stop_all_music();
    void play_victory_music(Color color);
    void play_startup_music(Color color);
};

#endif // LOTRCHESS_MUSIC_PLAYER_HPP
