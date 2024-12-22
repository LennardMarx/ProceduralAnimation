#ifndef ANIMAL_H
#define ANIMAL_H

#include <UI.h>

struct Vec2 {
  float x;
  float y;
};

struct Head {
  Vec2 drawPoints[5];
};

struct DrawPoint {
  float left[2];
  float right[2];
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
  float drawPoints[2];
};

class Animal {
public:
  Animal();
  ~Animal();

  void followMouse(UI *ui);
  void moveBody(int link);
  void drawLinks(UI *ui);
  void calculateDrawPoints();

private:
  float linkLength;
  float minAngle;
  float speed;

  static const int numJoints = 12;
  Segment body[numJoints];
  DrawPoint drawPoints[numJoints];
  Head head;
};

#endif // ANIMAL_H
