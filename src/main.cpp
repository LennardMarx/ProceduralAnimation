#include <UI.h>
#include <animal.h>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

struct context {
  UI *ui = nullptr;
  Animal *animal = nullptr;

  context() {
    ui = new UI(800, 600); // Dynamically allocate
    animal = new Animal();
  }

  ~context() {
    delete ui; // Ensure cleanup
    delete animal;
  }
};

static void mainLoop(void *arg) {
  context *ctx = static_cast<context *>(arg);

  ctx->ui->clear();
  ctx->ui->eventChecks();

  ctx->animal->followMouse(ctx->ui);
  // SDL_RenderDrawLine(ctx->ui->getRenderer(), 0, 0, ctx->ui->mouseX,
  //                    ctx->ui->mouseY);

  SDL_RenderPresent(ctx->ui->getRenderer());
}

int main() {
  context ctx;

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(mainLoop, &ctx, -1, 1);
#else
  while (!ctx.ui->getQuit()) {
    mainLoop(&ctx);
  }
#endif

  return 0;
}
