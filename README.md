# Lord of the Rings: Chess

## TODO:
- board adapter to interface with prometheus board to generate moves -> need chessmove and probably a lot from defines too
- widget classes:
  - Title class -> figures out where within the page to go by itself, included as an object by all the viewers
  - Mute Button Class -> same deal with figuring out where to go, included as an object
  - Options Class -> Same deal
  - Back Button Class
  - Toggle Button: 
- Save / serialize game state -> for example the whose turn it is and the current postion
- read pgn or output from c-chess-cli to playback in real-time what was happening
- inheritance between view clases?
- inheritance between dialog clases?
- Flyweight pattern?
- Replay games
- multithreading for reading from engine + updating things on screen etc. etc.