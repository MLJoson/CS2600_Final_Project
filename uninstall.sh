#!/bin/bash

set -e

sudo rm -f /usr/local/lib/libSDL#*.so*
sudo rm -f /usr/local/lib/pkgconfig/sdl3.pc
sudo rm -rf /usr/local/include/SDL3
sudo rm -f /usr/local/bin/sdl3-config

sudo rm -f /usr/local/libSDL3_ttf*.so*
sudo rm -f /usr/local/lib/pkgconfig/sdl3-ttf.pc
sudo rm -rf /usr/local/include/SDL3_ttf

rm -rf $HOME/.local/include/SDL3
rm -f $HOME/.local/include/SDL3_ttf.h
rm -f $HOME/.local/include/SDL_ttf.h

rm -rf $HOME/.local/lib/libSDL3*
rm -f $HOME/.local/lib/libSDL3_ttf.h
rm -f $HOME/.local/lib/pkgconfig/sdl3*.pc
rm -f $HOME/.local/lib/pkgconfig/SDL3*.pc
rm -f $HOME/.local/lib/cmake/SDL3*/ -r 2>/dev/null || true
rm -f $HOME/.local/lib/cmake/SDL3_ttf*/ -r 2>/dev/null || true

rm -rf $HOME/sdl3_build

echo "Uninstalled all SDL3 and SDL3_ttf libraries."
