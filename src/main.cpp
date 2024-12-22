#include <FABRIK.h>
#include <UI.h>
#include <animal.h>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

struct context {
  UI *ui = nullptr;
  Animal *animal = nullptr;
  RobotArm *robotArm = nullptr;
  FABRIK *fabrik = nullptr;

  context() {
    ui = new UI(800, 600); // Dynamically allocate
    animal = new Animal();
    robotArm = new RobotArm();
    fabrik = new FABRIK();
  }

  ~context() {
    delete ui; // Ensure cleanup
    delete animal;
    delete robotArm;
    delete fabrik;
  }
};

static void mainLoop(void *arg) {
  context *ctx = static_cast<context *>(arg);

  ctx->ui->clear();
  ctx->ui->eventChecks();

  // if (!ctx->fabrik->isSet) {
  //   ctx->fabrik->setTarget({150, 250});
  //   ctx->fabrik->isSet = true;
  // }
  //
  // ctx->fabrik->nextTarget = {ctx->fabrik->base.x, ctx->fabrik->base.y - 100};
  // // distance between taget and next target
  // float dx = ctx->fabrik->target.x - ctx->fabrik->nextTarget.x;
  // float dy = ctx->fabrik->target.y - ctx->fabrik->nextTarget.y;
  // float distance = sqrt(dx * dx + dy * dy);
  // if (distance > 50) {
  //   ctx->fabrik->setTarget(ctx->fabrik->nextTarget);
  // }
  // ctx->fabrik->setBase({(float)ctx->ui->mouseX, (float)ctx->ui->mouseY});
  // ctx->fabrik->solve(ctx->robotArm->joints);
  // ctx->robotArm->draw(ctx->ui);
  // ctx->fabrik->draw(ctx->ui);

  ctx->animal->followMouse(ctx->ui);

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
