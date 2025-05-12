#!/bin/bash

set -e

echo "Installing cmake to build libraries: "
sudo apt update
sudo apt install -y cmake git build-essential libfreetype6-dev libx11-dev libxext-dev libxrandr-dev pkg-config

mkdir -p ~/sdl3_build
cd ~/sdl3_build

echo "Cloning SDL3: "
git clone https://github.com/libsdl-org/SDL.git
cd SDL
rm -rf build
mkdir build && cd build

cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local -DSDL_INSTALL_INCLUDEDIR=include/SDL3

make -j$(nproc)
make install
cd ~/sdl3_build

echo "Cloning SDL3_ttf: "
git clone https://github.com/libsdl-org/SDL_ttf.git
cd SDL_ttf

rm -rf build
mkdir build && cd build

cmake .. \
      -DCMAKE_INSTALL_PREFIX=$HOME/.local \
      -DSDL3TTF=ON \
      -DCMAKE_PREFIX_PATH=$HOME/.local

make -j$(nproc)
make install
cd ../..

echo "Completed installation."
