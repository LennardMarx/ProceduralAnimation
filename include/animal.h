#ifndef ANIMAL_H
#define ANIMAL_H

#include <FABRIK.h>
#include <UI.h>
#include <Vec2.h>
#include <cmath>

struct DrawPoint {
  Vec2 left;
  Vec2 right;
  // float left[2];
  // float right[2];
};

struct Leg {
  Vec2 pos[3];
  Vec2 target;
  Vec2 elbowTarget;
  float angles[2];
  float length;
  DrawPoint drawPoints[4];
  FABRIK fabrik;
};

struct Head {
  Vec2 drawPoints[5];
};

struct Segment {
  Segment() {
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    angle = 0.0f;
  }
  float pos[2];
  float angle;
  float size;
  // float drawPoints[2];
  // Vec2 drawPoints;
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

private:
  float linkLength;
  float minAngle;
  float speed;

  static const int numJoints = 20;
  Segment body[numJoints];
  DrawPoint drawPoints[numJoints];
  Head head;
  Leg legs[4];
};

#endif // ANIMAL_H
