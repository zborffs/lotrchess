/// stl includes
#include <array>

/// project includes
#include "music_player.hpp"
#include "lotrchess.hpp"
#include "play_mode.hpp"
#include "splash_mode.hpp"

int main() {
    /// initialize constant variables
    constexpr auto width = 1280;
    constexpr auto height = 1080;
    constexpr auto title = "Lord of the Rings: Chess";
    constexpr auto fps_limit = 30;
    constexpr auto scale_factor = 2.0;
    constexpr auto font_scale_factor = 1.0;

    WindowData wd(width, height, fps_limit, scale_factor, font_scale_factor, title);

    const std::array<std::string, 4> music_paths({
        "../../res/music/white_startup_music.wav",
        "../../res/music/white_victory_music.wav",
        "../../res/music/black_startup_music.wav",
        "../../res/music/black_victory_music.wav",
    });

    LOTRChess lotrchess(wd, music_paths, std::make_unique<PlayMode>());
    lotrchess();

    return 0;
}