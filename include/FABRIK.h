#pragma once
#ifndef FABRIK_H
#define FABRIK_H

#include <UI.h>
#include <animal.h>

struct Link {
  Vec2 start;
  Vec2 end;
};

class RobotArm {
public:
  RobotArm() {
    links[0].start = {200, 200};
    links[0].end = {300, 200};
    links[1].start = {300, 200};
    links[1].end = {400, 200};
  }
  ~RobotArm() {}

  void draw(UI *ui) {
    for (int i = 0; i < 2; i++) {
      SDL_RenderDrawLine(ui->getRenderer(), links[i].start.x, links[i].start.y,
                         links[i].end.x, links[i].end.y);
    }
  }

  Link links[2];

private:
};

class FABRIK {
public:
  FABRIK() {}
  ~FABRIK() {}

  void solve(RobotArm *arm) {
    for (int i = 0; i < 10; i++) {
      pullToTraget(arm);
      pullToBase(arm);
    }
  }
  void setTarget(Vec2 t) { target = t; }
  void setBase(Vec2 b) { base = b; }
  void pullToTraget(RobotArm *arm) {
    arm->links[1].end = target;
    float dx = arm->links[1].end.x - arm->links[1].start.x;
    float dy = arm->links[1].end.y - arm->links[1].start.y;
    float distance = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx);

    arm->links[1].start.x = arm->links[1].end.x - cos(angle) * 100;
    arm->links[1].start.y = arm->links[1].end.y - sin(angle) * 100;

    arm->links[0].end = arm->links[1].start;

    dx = arm->links[0].end.x - arm->links[0].start.x;
    dy = arm->links[0].end.y - arm->links[0].start.y;
    distance = sqrt(dx * dx + dy * dy);
    angle = atan2(dy, dx);

    arm->links[0].start.x = arm->links[0].end.x - cos(angle) * 100;
    arm->links[0].start.y = arm->links[0].end.y - sin(angle) * 100;
  }
  void pullToBase(RobotArm *arm) {
    arm->links[0].start = base;
    float dx = arm->links[0].end.x - arm->links[0].start.x;
    float dy = arm->links[0].end.y - arm->links[0].start.y;
    float distance = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx);

    arm->links[0].end.x = arm->links[0].start.x + cos(angle) * 100;
    arm->links[0].end.y = arm->links[0].start.y + sin(angle) * 100;

    arm->links[1].start = arm->links[0].end;

    dx = arm->links[1].end.x - arm->links[1].start.x;
    dy = arm->links[1].end.y - arm->links[1].start.y;
    distance = sqrt(dx * dx + dy * dy);
    angle = atan2(dy, dx);

    arm->links[1].end.x = arm->links[1].start.x + cos(angle) * 100;
    arm->links[1].end.y = arm->links[1].start.y + sin(angle) * 100;
  }

  void draw(UI *ui) {
    // SDL_RenderDrawLine(ui->getRenderer(), base.x, base.y, target.x,
    // target.y);
    ui->DrawCircle(base.x, base.y, 10);
    ui->DrawCircle(target.x, target.y, 5);
  }

  RobotArm *arm;
  Vec2 base;
  Vec2 target;
  bool isSet = 0;

private:
};

#endif // FABRIK_H
