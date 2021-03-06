/// globals must be included first
#include "globals.hpp"

/// stl includes
#include <memory>

/// project includes
#include "lotrchess.hpp"
#include "play_screen.hpp"

int main() {
    LOTRChess app(std::make_unique<SplashScreen>()); // create the client app
    app(); // run the client app
    return 0;
}