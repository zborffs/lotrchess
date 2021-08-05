#ifndef LOTRCHESS_SPLASH_SCREEN_HPP
#define LOTRCHESS_SPLASH_SCREEN_HPP

#include "battle_setup_screen.hpp"
#include "lotrchess.hpp"
#include "play_mode.hpp"
#include "splash_dialog.hpp"
#include "splash_dialog_view.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <spdlog/spdlog.h>

class SplashScreen : public Screen {
    SplashDialog dialog_;
    SplashDialogView dialog_view_;

public:
    explicit SplashScreen() {
        spdlog::info("Created SplashScreen");
    }

    ~SplashScreen() {
        spdlog::info("Destroyed SplashScreen");
    }

    void process_event(sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LOTRCHESS_SPLASH_SCREEN_HPP
