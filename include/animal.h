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
  std::deque<Segment> segments;

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
  void moveLegs2();
  void drawLegs(UI *ui);

  void attachLegs();
  void setTargets(UI *ui);
  // std::deque<Segment> body;

  Leg legs[4];

private:
  // std::deque<Segment> leg[4];
  float linkLength;
  float minAngle;
  float speed;

  int legBase[4]{5, 5, 12, 12};

  static const int numJoints = 20;
  Head head;
  SpineSegment body[numJoints];
};

#endif // ANIMAL_H
