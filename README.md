Ludo Game (C++ Console Edition)

A 2-player Ludo game that runs in the Windows console, built with plain C++ and the Windows API (windows.h). Features a full 52-square board with colored token rendering, animated dice rolls with sound, save/load support, and a cheat mode.

Features


2-player local play — enter names for Player 1 (Red) and Player 2 (Yellow)
Full board rendering — 15x15 grid drawn directly in the console with color-coded tokens, home bases, and safe squares
Animated dice roll — cycling numbers with Beep() sound effects; rolling a 6 grants an extra turn
Token movement — leave home on a 6, move around the 52-square track, and enter the home stretch
Capturing — landing on an opponent's token (off safe squares) sends it back home
Save / Load — save mid-game to savegame.dat and resume later from the main menu
Cheat mode — press C during your turn to instantly move all your tokens to the final stretch (for testing)


Requirements


Windows OS (uses <windows.h> for cursor positioning, console colors, and Beep())
A C++ compiler such as MSVC (Visual Studio) or MinGW (g++)


Building

Using g++ (MinGW):

bashg++ game.cpp -o ludo.exe

Using Visual Studio:
Open a Developer Command Prompt and run:

bashcl game.cpp

Or create a new C++ console project in Visual Studio and add game.cpp to it.

Running

bashludo.exe

How to Play


On launch, choose 1. New Game or 2. Quit (or resume automatically if a save file exists).
Enter names for Player 1 and Player 2.
On your turn, you'll see the controls:

ENTER — roll the dice
S — save the game and continue later
Q — quit (with confirmation; unsaved progress is lost)
C — cheat (moves all your tokens to one step from home)



Roll a 6 to move a token out of home base, or to move an already-active token.
If you have multiple movable tokens, you'll be prompted to pick which one to move.
Landing on an opponent's token (not on a safe square, marked *) sends it back to their home base.
Get all 4 tokens to the center to win the game.


Controls Summary

KeyActionEnterRoll the diceSSave gameQQuit gameCCheat (debug/testing shortcut)

File Structure

game.cpp        # Full game source (single-file)
savegame.dat    # Auto-generated save file (created when you save)

Notes


This game relies on Windows-specific console APIs, so it will not compile or run as-is on Linux/macOS without modification (e.g., replacing windows.h calls with a cross-platform library like ncurses or ANSI escape codes).
The cheat mode (C) is intended for testing/debugging the win condition and is not hidden — remove it before distributing a "clean" version of the game.
