#include <animal.h>
#include <iostream>

Animal::Animal() {
  linkLength = 30.0f;
  speed = 0.04f;
  minAngle = 60.0f * M_PI / 180.0f;
  // Initialize body segments
  float segmentSizes[12] = {25, 20, 20, 25, 30, 30, 20, 15, 10, 10, 5, 0};
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

  // print angles
  // for (int i = 0; i < sizeof(body) / sizeof(body[0]); i++) {
  //   std::cout << "Angle " << i << ": " << body[i].angle << std::endl;
  // }

  calculateDrawPoints();

  drawLinks(ui);
}

void Animal::moveBody(int link) {
  if (link >= sizeof(body) / sizeof(body[0])) {
    // std::cout << " Link : " << link << std::endl;
    body[link - 1].angle = body[link - 2].angle;
    return;
  }
  float dx = body[link - 1].pos[0] - body[link].pos[0];
  float dy = body[link - 1].pos[1] - body[link].pos[1];
  float distance = sqrt(dx * dx + dy * dy);

  body[link].pos[0] = body[link - 1].pos[0] - dx / distance * linkLength;
  body[link].pos[1] = body[link - 1].pos[1] - dy / distance * linkLength;

  body[link - 1].angle = atan2(dy, dx);

  moveBody(link + 1);
}

void Animal::calculateDrawPoints() {
  for (int i = 0; i < sizeof(body) / sizeof(body[0]); i++) {
    drawPoints[i].left[0] =
        body[i].pos[0] + body[i].size * cos(body[i].angle + M_PI / 2);
    drawPoints[i].left[1] =
        body[i].pos[1] + body[i].size * sin(body[i].angle + M_PI / 2);
    drawPoints[i].right[0] =
        body[i].pos[0] + body[i].size * cos(body[i].angle - M_PI / 2);
    drawPoints[i].right[1] =
        body[i].pos[1] + body[i].size * sin(body[i].angle - M_PI / 2);
  }
  // Head
  head.drawPoints[0].x =
      body[0].pos[0] + body[0].size * cos(body[0].angle + M_PI / 3);
  head.drawPoints[0].y =
      body[0].pos[1] + body[0].size * sin(body[0].angle + M_PI / 3);
  head.drawPoints[1].x =
      body[0].pos[0] + body[0].size * cos(body[0].angle + M_PI / 6);
  head.drawPoints[1].y =
      body[0].pos[1] + body[0].size * sin(body[0].angle + M_PI / 6);
  head.drawPoints[2].x = body[0].pos[0] + body[0].size * cos(body[0].angle);
  head.drawPoints[2].y = body[0].pos[1] + body[0].size * sin(body[0].angle);
  head.drawPoints[3].x =
      body[0].pos[0] + body[0].size * cos(body[0].angle - M_PI / 6);
  head.drawPoints[3].y =
      body[0].pos[1] + body[0].size * sin(body[0].angle - M_PI / 6);
  head.drawPoints[4].x =
      body[0].pos[0] + body[0].size * cos(body[0].angle - M_PI / 3);
  head.drawPoints[4].y =
      body[0].pos[1] + body[0].size * sin(body[0].angle - M_PI / 3);
}

void Animal::drawLinks(UI *ui) {
  // Draw spine
  // for (int i = 0; i < sizeof(body) / sizeof(body[0]) - 1; i++) {
  //   SDL_RenderDrawLine(ui->getRenderer(), body[i].pos[0], body[i].pos[1],
  //                      body[i + 1].pos[0], body[i + 1].pos[1]);
  // }

  // Draw circle at each joint
  // for (int i = 0; i < sizeof(body) / sizeof(body[0]); i++) {
  //   ui->DrawCircle(body[i].pos[0], body[i].pos[1], body[i].size);
  // }

  // Draw red dot 90 degrees from each joint
  // for (int i = 0; i < sizeof(body) / sizeof(body[0]); i++) {
  //   SDL_RenderDrawPoint(ui->getRenderer(), drawPoints[i].left[0],
  //                       drawPoints[i].left[1]);
  //   SDL_RenderDrawPoint(ui->getRenderer(), drawPoints[i].right[0],
  //                       drawPoints[i].right[1]);
  // }

  // Connect draw points on each side
  for (int i = 0; i < sizeof(body) / sizeof(body[0]) - 1; i++) {
    SDL_RenderDrawLine(ui->getRenderer(), drawPoints[i].left[0],
                       drawPoints[i].left[1], drawPoints[i + 1].left[0],
                       drawPoints[i + 1].left[1]);
    SDL_RenderDrawLine(ui->getRenderer(), drawPoints[i].right[0],
                       drawPoints[i].right[1], drawPoints[i + 1].right[0],
                       drawPoints[i + 1].right[1]);
  }
  // draw circle at head
  // ui->DrawCircle(body[0].pos[0], body[0].pos[1], body[0].size);
  // draw head
  SDL_RenderDrawLine(ui->getRenderer(), drawPoints[0].left[0],
                     drawPoints[0].left[1], head.drawPoints[0].x,
                     head.drawPoints[0].y);
  SDL_RenderDrawLine(ui->getRenderer(), head.drawPoints[0].x,
                     head.drawPoints[0].y, head.drawPoints[1].x,
                     head.drawPoints[1].y);
  SDL_RenderDrawLine(ui->getRenderer(), head.drawPoints[1].x,
                     head.drawPoints[1].y, head.drawPoints[2].x,
                     head.drawPoints[2].y);
  SDL_RenderDrawLine(ui->getRenderer(), head.drawPoints[2].x,
                     head.drawPoints[2].y, head.drawPoints[3].x,
                     head.drawPoints[3].y);
  SDL_RenderDrawLine(ui->getRenderer(), head.drawPoints[3].x,
                     head.drawPoints[3].y, head.drawPoints[4].x,
                     head.drawPoints[4].y);
  SDL_RenderDrawLine(ui->getRenderer(), head.drawPoints[4].x,
                     head.drawPoints[4].y, drawPoints[0].right[0],
                     drawPoints[0].right[1]);
}
