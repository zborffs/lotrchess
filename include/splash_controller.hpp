#ifndef LOTRCHESS_SPLASH_CONTROLLER_HPP
#define LOTRCHESS_SPLASH_CONTROLLER_HPP

#include "defines.hpp"

class SplashController {
private:
    SplashSelector selection_;

public:
    explicit SplashController();

    [[nodiscard]] inline SplashSelector selection() noexcept {
        return selection_;
    }

    [[nodiscard]] inline SplashSelector selection() const noexcept {
        return selection_;
    }

    void set_selection(const SplashSelector& selection) {
        selection_ = selection;
    }
};

#endif // LOTRCHESS_SPLASH_CONTROLLER_HPP
