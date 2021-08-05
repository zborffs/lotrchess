#ifndef LOTRCHESS_SPLASH_MODE_HPP
#define LOTRCHESS_SPLASH_MODE_HPP

#include "lotrchess.hpp"
#include "splash_dialog.hpp"
#include "splash_dialog_view.hpp"
#include "play_mode.hpp"
#include "battle_setup_mode.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <spdlog/spdlog.h>

class SplashMode : public Mode {
    SplashDialog dialog_;
    SplashDialogView dialog_view_;

public:
    explicit SplashMode() {
        spdlog::info("Created SplashMode");
    }

    ~SplashMode() {
        spdlog::info("Destroyed SplashMode");
    }

    void process_event(sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LOTRCHESS_SPLASH_MODE_HPP
