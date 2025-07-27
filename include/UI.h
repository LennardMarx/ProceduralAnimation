#ifndef UI_H
#define UI_H

#include "structs.h"
#include <SDL2/SDL.h>
// #include <SDL2/SDL2_gfxPrimitives.h>

class UI {
public:
  const int sizeX;
  const int sizeY;

  int mouseX = 0;
  int mouseY = 0;

  UI(int, int);
  ~UI();

  void clear();
  void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius);
  void DrawArc(int32_t centreX, int32_t centreY, int32_t radius,
               float startAngle, float endAngle);
  void DrawArc(Vec2 center, float radius, float startAngle, float endAngle);
  SDL_Renderer *&getRenderer(); // pointer reference to the renderer
  SDL_Window *getWindow();      // pointer to the window
  bool getQuit();
  void eventChecks();

private:
  void initialize(int, int);

private:
  SDL_Window *window = nullptr;     // create window pointer
  SDL_Renderer *renderer = nullptr; // create renderer pointer

  // TexturedRectangle birdTex;

  bool quit = false;
  bool pause = false;
  SDL_Event event;
};

#endif // UI_H
