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
  FABRIK *fabrik = nullptr;
  std::deque<Segment> segments;

  context() {
    ui = new UI(1200, 800); // Dynamically allocate
    animal = new Animal();
    fabrik = new FABRIK();

    // addSegment(segments, std::make_shared<Vec2>(Vec2{0, 0}), 30, 0);
    // addSegment(segments, std::make_shared<Vec2>(Vec2{0, 0}), 30, 0);
    // addSegment(segments, std::make_shared<Vec2>(Vec2{0, 0}), 30, 0);

    for (int i = 0; i < 3; i++) {
      segments.push_back(Segment{Vec2{100, 100}, Vec2{200, 200}, 30, 0});
    }
  }

  ~context() {
    delete ui; // Ensure cleanup
    delete animal;
    delete fabrik;
  }
};

static void mainLoop(void *arg) {
  context *ctx = static_cast<context *>(arg);

  ctx->ui->clear();
  ctx->ui->eventChecks();

  float mouseXX = ctx->ui->mouseX;
  float mouseYY = ctx->ui->mouseY;
  ctx->fabrik->setTarget(Vec2{mouseXX, mouseYY});
  // ctx->fabrik->setTarget(Vec2{300, 300});
  ctx->fabrik->setBase(Vec2{200, 200});
  ctx->fabrik->solve(ctx->segments);

  for (int i = 0; i < ctx->segments.size(); i++) {
    SDL_RenderDrawLine(ctx->ui->getRenderer(), ctx->segments[i].start.x,
                       ctx->segments[i].start.y, ctx->segments[i].end.x,
                       ctx->segments[i].end.y);
  }
  // ctx->ui->DrawCircle(ctx->segments[0].start.x, ctx->segments[0].start.y,
  // 10); ctx->ui->DrawCircle(ctx->segments[2].end.x, ctx->segments[2].end.y,
  // 10); ctx->ui->DrawCircle(ctx->segments[1].end.x, ctx->segments[1].end.y,
  // 10); ctx->ui->DrawCircle(ctx->segments[1].start.x,
  // ctx->segments[1].start.y, 10);

  // std::cout << "Start: " << ctx->segments[0].start.x << ", "
  //           << ctx->segments[0].start.y << std::endl;
  // std::cout << "End: " << ctx->segments[2].end.x << ", "
  //           << ctx->segments[2].end.y << std::endl;

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
