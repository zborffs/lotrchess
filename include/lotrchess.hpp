#ifndef LOTRCHESS_LOTRCHESS_HPP
#define LOTRCHESS_LOTRCHESS_HPP

/// third party includes
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

/// stl includes
#include <array>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

/// project includes
#include "defines.hpp"
#include "music_player.hpp"

class LOTRChess; // context

class Mode {
protected:
    LOTRChess* context_;

public:
    virtual ~Mode() = default;
    virtual void process_event(sf::Event& event) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    void set_context(LOTRChess* context) {
        this->context_ = context;
    }
};

class LOTRChess {
protected:
    friend class PlayMode;
    friend class SplashMode;
    friend class BattleSetupMode;

private:
    std::unique_ptr<Mode> mode_;
    std::vector<Mode> saved_modes_{};
    sf::RenderWindow window_;
    sf::Clock delta_clock_;
    MusicPlayer music_player_; // put maybe in parent gamestate class

public:
    explicit LOTRChess(const WindowData& wd, const std::array<std::string, 4>& music_paths, std::unique_ptr<Mode> init_mode);

    ~LOTRChess() {
        ImGui::SFML::Shutdown(); // shut everything down
        spdlog::info("Destroyed LOTRChess");
    }
    void transition_to(Mode* mode);
    void operator()();
};

#endif // LOTRCHESS_LOTRCHESS_HPP
