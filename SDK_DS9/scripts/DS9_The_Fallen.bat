@ECHO OFF
call ChangeLinks.bat ds9_the_fallen
python SetProperties.py --debug-exe DS9.exe --no-unicode --game-define DEEP_SPACE_NINE
echo #define GAME_NAME "Star Trek: Deep Space Nine - The Fallen v1.0"> ..\gamename.h
