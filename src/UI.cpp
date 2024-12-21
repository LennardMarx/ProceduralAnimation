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
