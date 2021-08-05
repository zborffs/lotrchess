#include "lotrchess.hpp"

LOTRChess::LOTRChess(const WindowData& wd, const std::array<std::string, 4>& music_paths, std::unique_ptr<Mode> init_mode) : mode_(std::move(init_mode)), window_(sf::VideoMode(wd.width, wd.height), wd.title), music_player_(music_paths) {
    mode_->set_context(this);

    /// create the window object and set the max frame rate
    window_.setFramerateLimit(wd.fps_limit);
    ImGui::SFML::Init(window_);

    /// scale
    ImGui::GetStyle().ScaleAllSizes(wd.scale_factor);
    ImGui::GetIO().FontGlobalScale = wd.font_scale_factor;

//    const std::array<std::string, 4> music_paths({
//        "../../res/music/white_startup_music.wav",
//        "../../res/music/white_victory_music.wav",
//        "../../res/music/black_startup_music.wav",
//        "../../res/music/black_victory_music.wav",
//    });

//    /// initialize constant variables
//    constexpr auto width = 1280;
//    constexpr auto height = 1080;
//    constexpr auto title = "Lord of the Rings: Chess";
//    constexpr auto fps_limit = 30;
//    constexpr auto scale_factor = 2.0;
//    constexpr auto font_scale_factor = 1.0;

    spdlog::info("Created LOTRChess");
}


void LOTRChess::transition_to(Mode* mode) {
    mode_.reset(mode);
    mode_->set_context(this);
}

void LOTRChess::operator()() {
  /// main game-loop
  while (window_.isOpen()) {
      // while the window is still open, process the events and draw the objects


      sf::Event event;
      while (window_.pollEvent(event)) {
          mode_->process_event(event);
      }

      ImGui::SFML::Update(window_, delta_clock_.restart()); // let SFML update the window

      //        ImGui::Begin("The Plan!");
      //        for (std::size_t index{0}; const auto& step : steps) {
      //            ImGui::Checkbox(fmt::format("{} : {}", index, step).c_str(), &states.at(index));
      //            ++index;
      //        }
      //        ImGui::End();

      window_.clear(); // clear the window
      ImGui::SFML::Render(window_); // render the window
      mode_->draw(window_);
      mode_->draw(window_);
      window_.display(); // display the window
  }
}