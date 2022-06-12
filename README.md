# Lord of the Rings: Chess
This is a Lord of the Rings-themed chess game made using SFML. 

## Art Contributions
Huge thanks to Erica Hanania for making the board and piece sprites

## Source Dependencies
The project uses [conan](https://docs.conan.io/en/latest/introduction.html) to handle all the source dependencies. The 
code relies on the following libraries:
  1. boost.process ~ talking to the uci engine using pipes
  2. SFML+ImGui ~ Graphics + UI components
  3. spdlog ~ logging back to the programmer

## Todo List:
- pick promoting piece (user) -> another screen + event processing without losing gamestate for Play?
  - easiest way of doing this would be to simply add another variable to the playscreen class or something, determining
    whether or not we are in the promotion pick state. If we are not, then act as usual, but if we are, then depict 
    another screen and have new update actions?
- GUI for engine debugging (alla tord's program)
  - main screen that has a engine debugging option.
  - goes to playscreen but has additional buttons on the playscreen sidebar area
  - buttons include things like search this position (maybe even just a little text thing where you can send uci 
  commands)
  - after doing a search, make moves and inspect the transposition hash entries
  - area for loading pgns
  - area for loading FENS (just the same as the uci command area...)

## Troubleshooting
1. Conan Problems: https://docs.conan.io/en/latest/faq/troubleshooting.html
