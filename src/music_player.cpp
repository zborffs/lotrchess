#include "music_player.hpp"
MusicPlayer::MusicPlayer(const std::array<std::string, 4>& music_paths) {
    startup_music_[0].openFromFile(music_paths[0]); // white startup
    victory_music_[0].openFromFile(music_paths[1]); // white victory

    startup_music_[1].openFromFile(music_paths[2]); // black startup
    victory_music_[1].openFromFile(music_paths[3]); // black victory
}

void MusicPlayer::stop_all_music() {
    for (auto& m : startup_music_) {
        m.stop();
    }

    for (auto& m : victory_music_) {
        m.stop();
    }
}

void MusicPlayer::play_victory_music(Color color) {
    stop_all_music();

    spdlog::info("Playing {} victory music...", color == WHITE ? "white" : "black");

    auto offset = victory_music_[color].getPlayingOffset();
    victory_music_[color].play();
    victory_music_[color].setPlayingOffset(offset);
}

void MusicPlayer::play_startup_music(Color color) {
    stop_all_music();

    spdlog::info("Playing {} startup music...", color == WHITE ? "white" : "black");

    auto offset = startup_music_[color].getPlayingOffset();
    startup_music_[color].play();
    startup_music_[color].setPlayingOffset(offset);
}
