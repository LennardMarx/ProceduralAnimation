#include <animal.h>
#include <iostream>

Animal::Animal() {
  linkLength = 30.0f;
  speed = 0.05f;
  minAngle = 30.0f * M_PI / 180.0f;
  // Initialize body segments
  float segmentSizes[10] = {25, 20, 20, 25, 30, 30, 20, 15, 10, 10};
  for (int i = 0; i < sizeof(body) / sizeof(body[0]); i++) {
    body[i].pos[0] = 300.0f;
    body[i].pos[1] = i * -linkLength; // Stack segments vertically
    body[i].size = segmentSizes[i];
  }
}
Animal::~Animal() {}

void Animal::followMouse(UI *ui) {
  // First segment follows the mouse
  body[0].pos[0] += (ui->mouseX - body[0].pos[0]) * speed;
  body[0].pos[1] += (ui->mouseY - body[0].pos[1]) * speed;

  moveBody(1);
  drawLinks(ui);
}

void Animal::moveBody(int link) {
  if (link >= sizeof(body) / sizeof(body[0])) {
    return;
  }
  float dx = body[link - 1].pos[0] - body[link].pos[0];
  float dy = body[link - 1].pos[1] - body[link].pos[1];
  float distance = sqrt(dx * dx + dy * dy);

  body[link].pos[0] = body[link - 1].pos[0] - dx / distance * linkLength;
  body[link].pos[1] = body[link - 1].pos[1] - dy / distance * linkLength;

  moveBody(link + 1);
}

void Animal::drawLinks(UI *ui) {
  for (int i = 0; i < sizeof(body) / sizeof(body[0]) - 1; i++) {
    SDL_RenderDrawLine(ui->getRenderer(), body[i].pos[0], body[i].pos[1],
                       body[i + 1].pos[0], body[i + 1].pos[1]);
  }
  // draw circle at each joint
  for (int i = 0; i < sizeof(body) / sizeof(body[0]); i++) {
    ui->DrawCircle(body[i].pos[0], body[i].pos[1], body[i].size);
  }
}
