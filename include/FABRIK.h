#pragma once
#ifndef FABRIK_H
#define FABRIK_H

#include <UI.h>
#include <Vec2.h>
// #include <animal.h>

// struct Link {
//   Vec2 start;
//   Vec2 end;
// };

class RobotArm {
public:
  RobotArm() {
    // links[0].start = {200, 200};
    // links[0].end = {300, 200};
    // links[1].start = {300, 200};
    // links[1].end = {400, 200};
    joints[0] = {200, 200};
    joints[1] = {300, 200};
    joints[2] = {400, 200};
  }
  ~RobotArm() {}

  void draw(UI *ui) {
    for (int i = 0; i < 2; i++) {
      // SDL_RenderDrawLine(ui->getRenderer(), links[i].start.x,
      // links[i].start.y,
      //                    links[i].end.x, links[i].end.y);
      SDL_RenderDrawLine(ui->getRenderer(), joints[i].x, joints[i].y,
                         joints[i + 1].x, joints[i + 1].y);
    }
  }

  // Link links[2];
  Vec2 joints[3];

private:
};

class FABRIK {
public:
  FABRIK() {}
  ~FABRIK() {}

  void solve(Vec2 joints[3]) {
    // for (int i = 0; i < 10; i++) {
    pullElbow(joints);
    for (int i = 0; i < 10; i++) {
      pullToTraget(joints);
      pullToBase(joints);
    }
    // }
  }
  void setTarget(Vec2 t) { target = t; }
  void setElbowTarget(Vec2 t) { elbowTarget = t; }
  void setBase(Vec2 b) { base = b; }
  // void pullToTraget(RobotArm *arm) {
  void pullToTraget(Vec2 joints[3]) {
    joints[2] = target;

    float dx = joints[2].x - joints[1].x;
    float dy = joints[2].y - joints[1].y;
    float distance = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx);

    joints[1].x = joints[2].x - cos(angle) * 30;
    joints[1].y = joints[2].y - sin(angle) * 30;

    dx = joints[1].x - joints[0].x;
    dy = joints[1].y - joints[0].y;
    distance = sqrt(dx * dx + dy * dy);
    angle = atan2(dy, dx);

    joints[0].x = joints[1].x - cos(angle) * 30;
    joints[0].y = joints[1].y - sin(angle) * 30;
  }
  // void pullToBase(RobotArm *arm) {
  void pullToBase(Vec2 joints[3]) {
    joints[0] = base;
    float dx = joints[1].x - joints[0].x;
    float dy = joints[1].y - joints[0].y;
    float distance = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx);

    joints[1].x = joints[0].x + cos(angle) * 30;
    joints[1].y = joints[0].y + sin(angle) * 30;

    dx = joints[2].x - joints[1].x;
    dy = joints[2].y - joints[1].y;
    distance = sqrt(dx * dx + dy * dy);
    angle = atan2(dy, dx);

    joints[2].x = joints[1].x + cos(angle) * 30;
    joints[2].y = joints[1].y + sin(angle) * 30;
  }
  void pullElbow(Vec2 joints[3]) {
    joints[1] = elbowTarget;
    float dx = joints[0].x - joints[1].x;
    float dy = joints[0].y - joints[1].y;
    float distance = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx);

    joints[0].x = joints[1].x - cos(angle) * 30;
    joints[0].y = joints[1].y - sin(angle) * 30;

    dx = joints[2].x - joints[1].x;
    dy = joints[2].y - joints[1].y;
    distance = sqrt(dx * dx + dy * dy);
    angle = atan2(dy, dx);

    joints[2].x = joints[1].x + cos(angle) * 30;
    joints[2].y = joints[1].y + sin(angle) * 30;
  }

  void draw(UI *ui) {
    // SDL_RenderDrawLine(ui->getRenderer(), base.x, base.y, target.x,
    // target.y);
    ui->DrawCircle(base.x, base.y, 10);
    ui->DrawCircle(target.x, target.y, 5);
    ui->DrawCircle(nextTarget.x, nextTarget.y, 3);
  }

  RobotArm *arm;
  Vec2 base;
  Vec2 target;
  Vec2 elbowTarget;
  Vec2 nextTarget;
  bool isSet = 0;

private:
};

#endif // FABRIK_H
