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
#include "extern.hpp"

class LOTRChess; // context

/**
 * parent class of Mode objects: contains logic for transitioning to the next
 * mode
 * - State structural design pattern
 */
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

/**
 * The main application
 * - maintains context of the current game mode.
 * - owns the rendering window
 */
class LOTRChess {
protected:
    friend class PlayMode;
    friend class SplashMode;
    friend class BattleSetupMode;

private:
    std::unique_ptr<Mode> mode_; // unique pointer to the current mode
    sf::RenderWindow window_; // the window to which the game is rendering
    sf::Clock delta_clock_; // keeps track of difference between renders
    MusicPlayer music_player_; // put maybe in parent gamestate class
    // GameState state_; // whats the current gamestate
    // ResourceFactory res_; //
    // SnapShots snap_shots_; // incremental updates to the gamestate needed to achieve the current gamestate

public:
    explicit LOTRChess(std::unique_ptr<Mode> init_mode);
    ~LOTRChess();
    void transition_to(Mode* mode);
    void operator()();
};

#endif // LOTRCHESS_LOTRCHESS_HPP
