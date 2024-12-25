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
  // FABRIK *fabrik = nullptr;
  FABRIK fabrik[4];
  std::deque<Segment> segments;

  context() {
    ui = new UI(1200, 800); // Dynamically allocate
    animal = new Animal();
    // fabrik = new FABRIK();

    // addSegment(segments, std::make_shared<Vec2>(Vec2{0, 0}), 30, 0);
    // addSegment(segments, std::make_shared<Vec2>(Vec2{0, 0}), 30, 0);
    // addSegment(segments, std::make_shared<Vec2>(Vec2{0, 0}), 30, 0);

    // for (int i = 0; i < 3; i++) {
    //   segments.push_back(Segment{Vec2{100, 100}, Vec2{200, 200}, 30, 0});
    // }
  }

  ~context() {
    delete ui; // Ensure cleanup
    delete animal;
    // delete fabrik;
  }
};

static void mainLoop(void *arg) {
  context *ctx = static_cast<context *>(arg);

  ctx->ui->clear();
  ctx->ui->eventChecks();

  // float mouseXX = ctx->ui->mouseX;
  // float mouseYY = ctx->ui->mouseY;
  // ctx->fabrik->setTarget(Vec2{mouseXX, mouseYY});
  // // ctx->fabrik->setTarget(Vec2{300, 300});
  //
  // ctx->fabrik->setBase(Vec2{200, 200});
  // ctx->fabrik->solve(ctx->segments);
  //
  // for (int i = 0; i < ctx->segments.size(); i++) {
  //   SDL_RenderDrawLine(ctx->ui->getRenderer(), ctx->segments[i].start.x,
  //                      ctx->segments[i].start.y, ctx->segments[i].end.x,
  //                      ctx->segments[i].end.y);
  // }

  ctx->animal->followMouse(ctx->ui);

  for (int i = 0; i < 4; i++) {
    // FABRIK fabrik;
    // float dx = ctx->fabrik->target.x - ctx->animal->legs[i].target.x;
    // float dy = ctx->fabrik->target.y - ctx->animal->legs[i].target.y;
    // float dx = ctx->fabrik->target.x -
    // ctx->animal->legs[i].segments.back().end.x; float dy =
    // ctx->fabrik->target.y - ctx->animal->legs[i].segments.back().end.y;
    float dx = ctx->animal->legs[i].target.x -
               ctx->animal->legs[i].segments.back().end.x;
    float dy = ctx->animal->legs[i].target.y -
               ctx->animal->legs[i].segments.back().end.y;

    float distance = sqrt(dx * dx + dy * dy);
    // std::cout << "Distance " << i << " - " << distance << std::endl;
    // ctx->ui->DrawCircle(ctx->animal->legs[i].target.x,
    //                     ctx->animal->legs[i].target.y, 5);
    // ctx->ui->DrawCircle(ctx->fabrik->target.x, ctx->fabrik->target.y, 5);
    ctx->ui->DrawCircle(ctx->animal->legs[i].target.x,
                        ctx->animal->legs[i].target.y, 5);
    ctx->ui->DrawCircle(ctx->animal->legs[i].segments.back().end.x,
                        ctx->animal->legs[i].segments.back().end.y, 10);

    if (distance > 40) {
      // ctx->fabrik->setTarget(ctx->animal->legs[i].target);
      ctx->fabrik[i].setTarget(ctx->animal->legs[i].target);
      // fabrik.setTarget(ctx->animal->legs[i].target);
    }
    // ctx->fabrik->setBase(ctx->animal->legs[i].segments.front().start);
    // ctx->fabrik->solve(ctx->animal->legs[i].segments);
    ctx->fabrik[i].setBase(ctx->animal->legs[i].segments.front().start);
    ctx->fabrik[i].solve(ctx->animal->legs[i].segments);
    // fabrik.setBase(ctx->animal->legs[i].segments.front().start);
    // fabrik.solve(ctx->animal->legs[i].segments);

    // ctx->ui->DrawCircle(fabrik.target.x, fabrik.target.y, 5);
  }

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
