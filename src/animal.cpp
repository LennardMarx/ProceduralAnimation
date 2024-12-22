#include <animal.h>
#include <iostream>

Animal::Animal() {
  linkLength = 30.0f;
  speed = 0.03f;
  minAngle = 30.0f * M_PI / 180.0f;
  // Initialize body segments
  float segmentSizes[sizeof(body) / sizeof(body[0])] = {25, 20, 20, 25, 30, 30,
                                                        20, 15, 10, 10, 5,  0};
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
  moveLegs();

  calculateDrawPoints();

  drawLinks(ui);
  drawLegs(ui);
}

void Animal::moveLegs() {
  legs[0].pos[0] = drawPoints[3].left;
  legs[1].pos[0] = drawPoints[3].right;
  legs[2].pos[0] = drawPoints[7].left;
  legs[3].pos[0] = drawPoints[7].right;
  // outside dependent on angle
  legs[0].target.x = body[3].pos[0] + 60 * cos(body[3].angle + M_PI / 3);
  legs[0].target.y = body[3].pos[1] + 60 * sin(body[3].angle + M_PI / 3);
  legs[1].target.x = body[3].pos[0] + 60 * cos(body[3].angle - M_PI / 3);
  legs[1].target.y = body[3].pos[1] + 60 * sin(body[3].angle - M_PI / 3);
  legs[2].target.x = body[7].pos[0] + 60 * cos(body[7].angle + M_PI / 3);
  legs[2].target.y = body[7].pos[1] + 60 * sin(body[7].angle + M_PI / 3);
  legs[3].target.x = body[7].pos[0] + 60 * cos(body[7].angle - M_PI / 3);
  legs[3].target.y = body[7].pos[1] + 60 * sin(body[7].angle - M_PI / 3);

  for (int i = 0; i < 4; i++) {
    // float dx = legs[i].pos[2].x - legs[i].target.x;
    // float dy = legs[i].pos[2].y - legs[i].target.y;
    // if (!fabrik.isSet) {
    //   fabrik.setTarget(legs[i].target);
    //   fabrik.isSet = true;
    // }
    float dx = legs[i].fabrik.target.x - legs[i].target.x;
    float dy = legs[i].fabrik.target.y - legs[i].target.y;
    float distance = sqrt(dx * dx + dy * dy);
    // std::cout << "Distance: " << distance << std::endl;
    if (distance > 60) {
      legs[i].fabrik.setTarget(legs[i].target);
    }
    legs[i].fabrik.setBase(legs[i].pos[0]);
    legs[i].fabrik.solve(legs[i].pos);
  }
}

void Animal::drawLegs(UI *ui) {
  for (int i = 0; i < 4; i++) {
    // ui->DrawCircle(legs[i].pos[0].x, legs[i].pos[0].y, 5);
    // ui->DrawCircle(legs[i].target.x, legs[i].target.y, 5);
    SDL_RenderDrawLine(ui->getRenderer(), legs[i].pos[0].x, legs[i].pos[0].y,
                       legs[i].pos[1].x, legs[i].pos[1].y);
    SDL_RenderDrawLine(ui->getRenderer(), legs[i].pos[1].x, legs[i].pos[1].y,
                       legs[i].pos[2].x, legs[i].pos[2].y);
  }
}

float Animal::normalizeAngle(float angle) {
  while (angle > M_PI)
    angle -= 2 * M_PI;
  while (angle < -M_PI)
    angle += 2 * M_PI;
  return angle;
}

void Animal::moveBody(int link) {
  if (link >= sizeof(body) / sizeof(body[0])) {
    body[link - 1].angle = normalizeAngle(body[link - 2].angle);
    return;
  }

  float dx = body[link - 1].pos[0] - body[link].pos[0];
  float dy = body[link - 1].pos[1] - body[link].pos[1];
  float distance = sqrt(dx * dx + dy * dy);

  body[link - 1].angle = normalizeAngle(atan2(dy, dx));

  // std::cout << "Link: " << link << " Angle: " << body[link - 1].angle
  //           << std::endl;

  if (link > 1) {
    float angleDiff =
        normalizeAngle(body[link - 1].angle - body[link - 2].angle);

    if (angleDiff > minAngle) {
      body[link - 1].angle = normalizeAngle(body[link - 2].angle + minAngle);
    } else if (angleDiff < -minAngle) {
      body[link - 1].angle = normalizeAngle(body[link - 2].angle - minAngle);
    }
  }

  body[link].pos[0] =
      body[link - 1].pos[0] - cos(body[link - 1].angle) * linkLength;
  body[link].pos[1] =
      body[link - 1].pos[1] - sin(body[link - 1].angle) * linkLength;

  moveBody(link + 1);
}

void Animal::calculateDrawPoints() {
  for (int i = 0; i < sizeof(body) / sizeof(body[0]); i++) {
    drawPoints[i].left.x =
        body[i].pos[0] + body[i].size * cos(body[i].angle + M_PI / 2);
    drawPoints[i].left.y =
        body[i].pos[1] + body[i].size * sin(body[i].angle + M_PI / 2);
    drawPoints[i].right.x =
        body[i].pos[0] + body[i].size * cos(body[i].angle - M_PI / 2);
    drawPoints[i].right.y =
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
    SDL_RenderDrawLine(ui->getRenderer(), drawPoints[i].left.x,
                       drawPoints[i].left.y, drawPoints[i + 1].left.x,
                       drawPoints[i + 1].left.y);
    SDL_RenderDrawLine(ui->getRenderer(), drawPoints[i].right.x,
                       drawPoints[i].right.y, drawPoints[i + 1].right.x,
                       drawPoints[i + 1].right.y);
  }

  // draw head
  SDL_RenderDrawLine(ui->getRenderer(), drawPoints[0].left.x,
                     drawPoints[0].left.y, head.drawPoints[0].x,
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
                     head.drawPoints[4].y, drawPoints[0].right.x,
                     drawPoints[0].right.y);

  // Eyes
  // SDL_RenderDrawPoint(
  //     ui->getRenderer(),
  //     body[0].pos[0] + body[0].size / 2 * cos(body[0].angle + M_PI / 3),
  //     body[0].pos[1] + body[0].size / 2 * sin(body[0].angle + M_PI / 3));
  // SDL_RenderDrawPoint(
  //     ui->getRenderer(),
  //     body[0].pos[0] + body[0].size / 2 * cos(body[0].angle - M_PI / 3),
  //     body[0].pos[1] + body[0].size / 2 * sin(body[0].angle - M_PI / 3));
  ui->DrawCircle(
      body[0].pos[0] + body[0].size / 2 * cos(body[0].angle + M_PI / 3),
      body[0].pos[1] + body[0].size / 2 * sin(body[0].angle + M_PI / 3), 3);
  ui->DrawCircle(
      body[0].pos[0] + body[0].size / 2 * cos(body[0].angle - M_PI / 3),
      body[0].pos[1] + body[0].size / 2 * sin(body[0].angle - M_PI / 3), 3);
}
