#ifndef ANIMAL_H
#define ANIMAL_H

#include <UI.h>
#include <iostream>

class Animal {
public:
  Animal() {
    head[0] = 0.0f;
    head[1] = 0.0f;
    tail[0] = 0.0f;
    tail[1] = 0.0f;
    length = 30.0f;
  }
  ~Animal() {}

  void followMouse(UI *ui) {
    head[0] = ui->mouseX;
    head[1] = ui->mouseY;
    moveTail();
    draw(ui);
  }

  // move tail in direction of head if distance is greater than length
  void moveTail() {
    float dx = head[0] - tail[0];
    float dy = head[1] - tail[1];
    float distance = sqrt(dx * dx + dy * dy);

    // instant movement
    if (distance > length) {
      tail[0] = head[0] - dx / distance * length;
      tail[1] = head[1] - dy / distance * length;
    }

    // if (distance > length) {
    //   tail[0] += dx / distance;
    //   tail[1] += dy / distance;
    // }
  }

  // draw head and tail
  void draw(UI *ui) {
    SDL_RenderDrawLine(ui->getRenderer(), head[0], head[1], tail[0], tail[1]);
  }

private:
  float head[2];
  float tail[2];
  float length;
};

#endif // ANIMAL_H
