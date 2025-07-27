#include <UI.h>

UI::UI(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY) {
  initialize(sizeX, sizeY);
}

UI::~UI() {
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
}

// TODO: Maybe get rid of wrappers
void UI::clear() {
  SDL_SetRenderDrawColor(renderer, 69, 133, 136, 255); // gruv-blue
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 249, 245, 215, 150); // grub-light
}

// void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY,
//                 int32_t radius) {
void UI::DrawCircle(int32_t centreX, int32_t centreY, int32_t radius) {
  const int32_t diameter = (radius * 2);

  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

void UI::DrawArc(Vec2 center, float radius, float startAngle, float endAngle) {
  // Wrap angle
  if (endAngle < startAngle)
    endAngle += 2 * M_PI;

  int degrees = (endAngle - startAngle) * 180 / M_PI;
  // int degrees =
  //     std::max(1, static_cast<int>((endAngle - startAngle) * 180 / M_PI));
  // start point
  float x_prev = center.x + radius * cos(startAngle) + 0.5;
  float y_prev = center.y + radius * sin(startAngle) + 0.5;
  // for (int i = 0; i < degrees; i++) {
  // for (int i = 0; i < degrees; i += 2) {
  for (int i = 0; i < degrees; i = i + 4) {
    float angle = startAngle + i * M_PI / 180;

    float x = center.x + radius * cos(angle) + 0.5;
    float y = center.y + radius * sin(angle) + 0.5;

    // SDL_RenderDrawLine(renderer, x_prev, y_prev, x, y);

    int x1 = static_cast<int>(x_prev);
    int y1 = static_cast<int>(y_prev);
    int x2 = static_cast<int>(x);
    int y2 = static_cast<int>(y);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

    x_prev = x;
    y_prev = y;
  }
}

void UI::DrawArc(int32_t centreX, int32_t centreY, int32_t radius,
                 float startAngle, float endAngle) {
  const int32_t diameter = (radius * 2);

  // Convert angles from degrees to radians
  float startRad = startAngle * M_PI / 180.0f;
  float endRad = endAngle * M_PI / 180.0f;

  // Variables for the circle drawing
  int32_t x = radius;
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y) {
    // Calculate the angle for the current (x, y) point
    float angle = atan2(y, x); // Get the angle in radians for this point

    // Check if the angle is within the range [startRad, endRad]
    if (angle >= startRad && angle <= endRad) {
      // Draw points only within the angle range
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
    }

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

SDL_Renderer *&UI::getRenderer() // pointer reference to the renderer
{
  return renderer;
}
SDL_Window *UI::getWindow() // pointer to the window
{
  return window;
}

// initializing the UI
void UI::initialize(int sizeX, int sizeY) {
  SDL_Init(SDL_INIT_EVERYTHING);

  // Create a Window
  window = SDL_CreateWindow("Drone Simulation", 0, 0, sizeX, sizeY,
                            SDL_WINDOW_SHOWN);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // scale window
  SDL_SetWindowSize(window, sizeX, sizeY);
  // adjust render scale
  SDL_RenderSetScale(renderer, 1, 1);
  // place window in middle of screen after scaling
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

bool UI::getQuit() { return quit; }

void UI::eventChecks() {
  while (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_QUIT) {
      quit = true;
    }
    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        quit = true;
        break;
      case SDLK_q:
        quit = true;
        break;
      case SDLK_SPACE:
        pause = !pause;
        break;
      default:
        break;
      }
    case SDL_MOUSEMOTION: // Handle mouse motion event
      SDL_GetMouseState(&mouseX, &mouseY);
      break;
    case SDL_MOUSEBUTTONDOWN: // Handle mouse button press
      SDL_GetMouseState(&mouseX, &mouseY);
      break;
    }
  }
}
