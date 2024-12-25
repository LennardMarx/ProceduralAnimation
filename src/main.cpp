#include <FABRIK.h>
#include <UI.h>
#include <animal.h>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

struct context {
  UI ui{1200, 800};
  Animal animal;
  FABRIK fabrik[4];
};

static void mainLoop(void *arg) {
  context *ctx = static_cast<context *>(arg);

  ctx->ui.clear();
  ctx->ui.eventChecks();

  ctx->animal.followMouse(&ctx->ui);

  for (int i = 0; i < 4; i++) {
    float dx = ctx->animal.legs[i].target.x -
               ctx->animal.legs[i].segments.back().end.x;
    float dy = ctx->animal.legs[i].target.y -
               ctx->animal.legs[i].segments.back().end.y;
    float distance = sqrt(dx * dx + dy * dy);

    // ctx->ui.DrawCircle(ctx->animal.legs[i].target.x,
    //                    ctx->animal.legs[i].target.y, 5);
    // ctx->ui.DrawCircle(ctx->animal.legs[i].segments.back().end.x,
    //                    ctx->animal.legs[i].segments.back().end.y, 10);

    // std::cout << "Distance: " << distance << std::endl;

    if (distance > 50) {
      ctx->fabrik[i].setTarget(ctx->animal.legs[i].target);
    }
    ctx->fabrik[i].setBase(ctx->animal.legs[i].segments.front().start);
    ctx->fabrik[i].solve(ctx->animal.legs[i].segments);
  }

  SDL_RenderPresent(ctx->ui.getRenderer());
}

int main() {
  context ctx;

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(mainLoop, &ctx, -1, 1);
#else
  while (!ctx.ui.getQuit()) {
    mainLoop(&ctx);
  }
#endif

  return 0;
}
