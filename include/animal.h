#ifndef ANIMAL_H
#define ANIMAL_H

#include "structs.h"
#include <FABRIK.h>
#include <UI.h>
#include <cmath>
// #include <list>
#include <deque>

struct DrawPoint {
  Vec2 left;
  Vec2 right;
};

struct Bone {
  Vec2 proximal;
  Vec2 distal;
  float angle;
  float length;
  DrawPoint drawPoints[2];

  Bone *next;
  Bone *prev;
};

struct Leg {
  Segment segments[2];

  Bone bones[2];
  Vec2 pos[3];
  float angles[2];
  float length;
  DrawPoint drawPoints[4];

  FABRIK fabrik;
  Vec2 target;
  Vec2 elbowTarget;
};

struct Head {
  Vec2 drawPoints[5];
};

struct SpineSegment {
  SpineSegment() {
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    angle = 0.0f;
  }
  float pos[2];
  float angle;
  float size;
  DrawPoint drawPoints;
};

class Animal {
public:
  Animal();
  ~Animal();

  void followMouse(UI *ui);
  void moveBody(int link);
  void drawLinks(UI *ui);
  void calculateDrawPoints();
  float normalizeAngle(float angle);
  void moveLegs();
  void drawLegs(UI *ui);

  // std::deque<Segment> body;
  std::deque<Segment> leg[4];

private:
  float linkLength;
  float minAngle;
  float speed;

  static const int numJoints = 20;
  Head head;
  SpineSegment body[numJoints];
  Leg legs[4];
};

#endif // ANIMAL_H
