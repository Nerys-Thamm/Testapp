@echo off
:: Script to download audio from youtube videos using youtube-dl, and convert to mp3 using FFMPEG
:: Nerys Thamm (c)2021
set arg1=%1
set arg2=%2
set arg3=%3
youtube-dl -f bestaudio --extract-audio --audio-format mp3 %arg1%^=%arg2% -o Resources/Audio/%arg3%.^%%(ext)s

