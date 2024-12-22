#ifndef ANIMAL_H
#define ANIMAL_H

#include <UI.h>

struct Segment {
  Segment() {
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    angle = 0.0f;
  }
  float pos[2];
  float angle;
  float size;
};

class Animal {
public:
  Animal();
  ~Animal();

  void followMouse(UI *ui);
  void moveBody(int link);
  void drawLinks(UI *ui);

private:
  float linkLength;
  float minAngle;
  float speed;

  static const int numJoints = 10;
  Segment body[numJoints];
};

#endif // ANIMAL_H
