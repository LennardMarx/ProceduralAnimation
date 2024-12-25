#pragma once
#ifndef FABRIK_H
#define FABRIK_H

#include "UI.h"
#include "structs.h"
#include <deque>
#include <iostream>
// #include <list>

class FABRIK {
public:
  FABRIK() {
    target = Vec2{0, 0};
    base = Vec2{0, 0};
    elbowTarget = Vec2{0, 0};
  }
  ~FABRIK() {}

  void solve(Vec2 joints[3]) {
    pullElbow(joints);
    for (int i = 0; i < 10; i++) {
      pullToTraget(joints);
      pullToBase(joints);
    }
  }

  void solve(std::deque<Segment> &segments) {
    for (int i = 0; i < 10; i++) {
      pullToTarget(segments);
      pullToBase(segments);
    }
  }

  void setTarget(Vec2 t) { target = t; }
  void setElbowTarget(Vec2 t) { elbowTarget = t; }
  void setBase(Vec2 b) { base = b; }

  void pullToTarget(std::deque<Segment> &segments) {
    segments.back().end = target;
    std::cout << "back(): " << segments.back().end.x << ", "
              << segments.back().end.y << std::endl;

    std::cout << "[]: " << segments[segments.size() - 1].end.x << ", "
              << segments[segments.size() - 1].end.y << std::endl;
    // std::cout << "Size: " << segments.size() << std::endl;
    for (int i = segments.size() - 1; i >= 0; i--) {
      float dx = segments[i].end.x - segments[i].start.x;
      float dy = segments[i].end.y - segments[i].start.y;
      float distance = sqrt(dx * dx + dy * dy);
      segments[i].angle = atan2(dy, dx);

      // std::cout << "Angle: " << segments[i].angle << std::endl;

      segments[i].start.x =
          segments[i].end.x - cos(segments[i].angle) * segments[i].length;
      segments[i].start.y =
          segments[i].end.y - sin(segments[i].angle) * segments[i].length;

      // Set end of next Segment to start of current Segment
      if (i > 0) {
        segments[i - 1].end = segments[i].start;
      }
    }
  }

  void pullToBase(std::deque<Segment> &segments) {
    segments.front().start = base;

    for (int i = 0; i < segments.size(); i++) {
      float dx = segments[i].end.x - segments[i].start.x;
      float dy = segments[i].end.y - segments[i].start.y;
      float distance = sqrt(dx * dx + dy * dy);
      segments[i].angle = atan2(dy, dx);

      segments[i].end.x =
          segments[i].start.x + cos(segments[i].angle) * segments[i].length;
      segments[i].end.y =
          segments[i].start.y + sin(segments[i].angle) * segments[i].length;

      // Set start of next Segment to end of current Segment
      if (i < segments.size() - 1) {
        segments[i + 1].start = segments[i].end;
      }
    }
  }

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

  Vec2 base;
  Vec2 target;
  Vec2 elbowTarget;
  Vec2 nextTarget;
  bool isSet = 0;

private:
};

#endif // FABRIK_H
