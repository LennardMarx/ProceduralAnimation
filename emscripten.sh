source ~/Software/emsdk/emsdk_env.sh
# emcc src/*.cpp -Iinclude/ -s WASM=1 -s USE_SDL=2 -s SDL2_IMAGE_FORMATS='["bmp"]' -s USE_SDL_TTF=2 --preload-file resources -O2 -o procedural_animation.js
emcc src/*.cpp -Iinclude/ -s WASM=1 -s USE_SDL=2 -s SDL2_IMAGE_FORMATS='["bmp"]' -s USE_SDL_TTF=2 -o procedural_animation.js
