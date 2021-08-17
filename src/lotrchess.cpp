#include "lotrchess.hpp"

/**
 * constructs a LOTRChess object given an initial mode
 * @param init_mode the mode to start in
 */
LOTRChess::LOTRChess(std::unique_ptr<Screen> init_mode)
        : screen_(std::move(init_mode)),
          window_(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE),
          music_player_(MUSIC_PATHS) {
    screen_->set_context(this); // set the context of the new state to 'this'

    /// boilerplate initialization of window objects
    window_.setFramerateLimit(FPS_LIMIT); // set the frame rate limit
    ImGui::SFML::Init(window_); // pass in window to ImGui
    ImGui::GetStyle().ScaleAllSizes(SCALE_FACTOR); // scale objects
    ImGui::GetIO().FontGlobalScale = FONT_SCALE_FACTOR; // scale fonts

    spdlog::info("Created LOTRChess..."); // log creation
}

/**
 * LOTRChess destructor
 */
LOTRChess::~LOTRChess() {
    ImGui::SFML::Shutdown(); // shut everything down
    spdlog::info("Destroyed LOTRChess...");
}

/**
 * transitions to another game mode (like splash mode, battle setup mode, playmode)
 * @param mode a pointer to the mode object to switch to
 */
void LOTRChess::transition_to(Screen *mode) {
    screen_.reset(mode); // destroy previous state and setup new state pointer
    screen_->set_context(this); // set the state context of the new state to 'this'
}

/**
 * launches the application by starting the game-loop
 */
void LOTRChess::operator()() {
    /// main game-loop
    while (window_.isOpen()) {
        // while the window is still open, process the events and draw the objects

        sf::Event event;
        while (window_.pollEvent(event)) {
            // process all the events by the current mode
            screen_->process_event(event);
        }

        ImGui::SFML::Update(window_, delta_clock_.restart()); // let SFML update the window
        screen_->update();

        window_.clear(); // clear the window
        ImGui::SFML::Render(window_); // render the window
        screen_->draw(window_);
        screen_->draw(window_);
        window_.display(); // display the window
    }
}

