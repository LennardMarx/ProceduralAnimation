#include <animal.h>
#include <iostream>

Animal::Animal() {
  linkLength = 15.0f;
  speed = 0.02f;
  minAngle = 20.0f * M_PI / 180.0f;

  // Initialize body segments
  float segmentSizes[sizeof(body) / sizeof(body[0])] = {
      27, 25, 22, 20, 23, 27, 30, 32, 32, 30,
      27, 23, 20, 15, 12, 10, 7,  5,  3,  0};

  for (int i = 0; i < sizeof(body) / sizeof(body[0]); i++) {
    body[i].pos[0] = 300.0f;
    body[i].pos[1] = i * -linkLength; // Stack segments vertically
    body[i].size = segmentSizes[i];
  }

  // Create spine
  for (int i = 0; i < sizeof(segmentSizes) / sizeof(segmentSizes[0]); i++) {
    spine.segments.push_back(
        Segment{Vec2{0, 0}, Vec2{0, 0}, linkLength, 0, segmentSizes[i]});
  }

  // Create legs
  for (int i = 0; i < sizeof(legs) / sizeof(legs[0]); i++) {
    // for (int j = 0; j < 2; j++) {
    //   legs[i].segments.push_back(Segment{Vec2{0, 0}, Vec2{0, 0}, 30, 0});
    // }
    legs[i].segments.push_back(Segment{Vec2{0, 0}, Vec2{0, 0}, 30, 0, 5});
    legs[i].segments.push_back(Segment{Vec2{0, 0}, Vec2{0, 0}, 30, 0, 5});
  }
}
Animal::~Animal() {}

void Animal::followMouse(UI *ui) {
  // First segment follows the mouse
  body[0].pos[0] += (ui->mouseX - body[0].pos[0]) * speed;
  body[0].pos[1] += (ui->mouseY - body[0].pos[1]) * speed;

  spine.segments[0].start.x += (ui->mouseX - spine.segments[0].start.x) * speed;
  spine.segments[0].start.y += (ui->mouseY - spine.segments[0].start.y) * speed;

  calculateDrawPoints();

  attachLegs();
  setTargets(ui);

  moveBody(1);
  moveSpine();
  // moveLegs();

  drawLinks(ui);
  drawLegs(ui);
  drawSpine(ui);
}

void Animal::attachLegs() {
  legs[0].segments.front().start = body[legBase[0]].drawPoints.left;
  legs[1].segments.front().start = body[legBase[1]].drawPoints.right;
  legs[2].segments.front().start = body[legBase[2]].drawPoints.left;
  legs[3].segments.front().start = body[legBase[3]].drawPoints.right;
  // std::cout << "Start: " << legs[0].segments.front().start.x << ", "
  //           << legs[0].segments.front().start.y << std::endl;
}

void Animal::setTargets(UI *ui) {
  const int DIST_FROM_SPINE = 50;
  legs[0].target =
      Vec2{body[legBase[0] - 2].pos[0] +
               cos(body[legBase[0] - 2].angle + M_PI / 2) * DIST_FROM_SPINE,
           body[legBase[0] - 2].pos[1] +
               sin(body[legBase[0] - 2].angle + M_PI / 2) * DIST_FROM_SPINE};
  legs[1].target =
      Vec2{body[legBase[1] - 2].pos[0] +
               cos(body[legBase[1] - 2].angle - M_PI / 2) * DIST_FROM_SPINE,
           body[legBase[1] - 2].pos[1] +
               sin(body[legBase[1] - 2].angle - M_PI / 2) * DIST_FROM_SPINE};
  legs[2].target =
      Vec2{body[legBase[2] - 2].pos[0] +
               cos(body[legBase[2] - 2].angle + M_PI / 2) * DIST_FROM_SPINE,
           body[legBase[2]].pos[1] +
               sin(body[legBase[2] - 2].angle + M_PI / 2) * DIST_FROM_SPINE};
  legs[3].target =
      Vec2{body[legBase[3] - 2].pos[0] +
               cos(body[legBase[3] - 2].angle - M_PI / 2) * DIST_FROM_SPINE,
           body[legBase[3] - 2].pos[1] +
               sin(body[legBase[3] - 2].angle - M_PI / 2) * DIST_FROM_SPINE};
  for (int i = 0; i < 4; i++) {
    // ui->DrawCircle(legs[i].target.x, legs[i].target.y, 5);
  }
}

void Animal::moveLegs() {
  // Leg attachement points
  legs[0].pos[0] = body[5].drawPoints.left;
  legs[1].pos[0] = body[5].drawPoints.right;
  legs[2].pos[0] = body[12].drawPoints.left;
  legs[3].pos[0] = body[12].drawPoints.right;

  // Targets for feet and elbows
  legs[0].target.x = body[5].pos[0] + 60 * cos(body[5].angle + M_PI / 3);
  legs[0].target.y = body[5].pos[1] + 60 * sin(body[5].angle + M_PI / 3);
  legs[1].target.x = body[5].pos[0] + 60 * cos(body[5].angle - M_PI / 3);
  legs[1].target.y = body[5].pos[1] + 60 * sin(body[5].angle - M_PI / 3);
  legs[2].target.x = body[12].pos[0] + 40 * cos(body[12].angle + M_PI / 2);
  legs[2].target.y = body[12].pos[1] + 40 * sin(body[12].angle + M_PI / 2);
  legs[3].target.x = body[12].pos[0] + 40 * cos(body[12].angle - M_PI / 2);
  legs[3].target.y = body[12].pos[1] + 40 * sin(body[12].angle - M_PI / 2);

  legs[0].elbowTarget.x = body[5].pos[0] + 40 * cos(body[5].angle + M_PI / 1.5);
  legs[0].elbowTarget.y = body[5].pos[1] + 40 * sin(body[5].angle + M_PI / 1.5);
  legs[1].elbowTarget.x = body[5].pos[0] + 40 * cos(body[5].angle - M_PI / 1.5);
  legs[1].elbowTarget.y = body[5].pos[1] + 40 * sin(body[5].angle - M_PI / 1.5);
  legs[2].elbowTarget.x = body[12].pos[0] + 50 * cos(body[12].angle + M_PI / 3);
  legs[2].elbowTarget.y = body[12].pos[1] + 50 * sin(body[12].angle + M_PI / 3);
  legs[3].elbowTarget.x = body[12].pos[0] + 50 * cos(body[12].angle - M_PI / 3);
  legs[3].elbowTarget.y = body[12].pos[1] + 50 * sin(body[12].angle - M_PI / 3);
}

void Animal::drawLegs(UI *ui) {
  for (int i = 0; i < 4; i++) {
    // Joints
    // ui->DrawCircle(legs[i].pos[0].x, legs[i].pos[0].y, 7);
    // ui->DrawCircle(legs[i].pos[1].x, legs[i].pos[1].y, 7);
    // ui->DrawCircle(legs[i].pos[2].x, legs[i].pos[2].y, 7);

    // Outline of legs
    // SDL_RenderDrawLine(ui->getRenderer(), legs[i].drawPoints[0].left.x,
    //                    legs[i].drawPoints[0].left.y,
    //                    legs[i].drawPoints[1].left.x,
    //                    legs[i].drawPoints[1].left.y);
    // SDL_RenderDrawLine(ui->getRenderer(), legs[i].drawPoints[0].right.x,
    //                    legs[i].drawPoints[0].right.y,
    //                    legs[i].drawPoints[1].right.x,
    //                    legs[i].drawPoints[1].right.y);
    // SDL_RenderDrawLine(ui->getRenderer(), legs[i].drawPoints[2].right.x,
    //                    legs[i].drawPoints[2].right.y,
    //                    legs[i].drawPoints[3].right.x,
    //                    legs[i].drawPoints[3].right.y);
    // SDL_RenderDrawLine(ui->getRenderer(), legs[i].drawPoints[2].left.x,
    //                    legs[i].drawPoints[2].left.y,
    //                    legs[i].drawPoints[3].left.x,
    //                    legs[i].drawPoints[3].left.y);

    for (int j = 0; j < legs[i].segments.size(); j++) {
      SDL_RenderDrawLine(ui->getRenderer(), legs[i].segments[j].start.x,
                         legs[i].segments[j].start.y, legs[i].segments[j].end.x,
                         legs[i].segments[j].end.y);
    }
  }
}

void Animal::drawSpine(UI *ui) {
  for (int i = 0; i < spine.segments.size(); i++) {
    SDL_RenderDrawLine(ui->getRenderer(), spine.segments[i].start.x,
                       spine.segments[i].start.y, spine.segments[i].end.x,
                       spine.segments[i].end.y);
    ui->DrawCircle(spine.segments[i].start.x, spine.segments[i].start.y,
                   spine.segments[i].size);
  }
}

float Animal::normalizeAngle(float angle) {
  while (angle > M_PI)
    angle -= 2 * M_PI;
  while (angle < -M_PI)
    angle += 2 * M_PI;
  return angle;
}

void Animal::moveSpine() {
  for (int i = 0; i < spine.segments.size(); i++) {
    float dx = spine.segments[i].start.x - spine.segments[i].end.x;
    float dy = spine.segments[i].start.y - spine.segments[i].end.y;
    float distance = sqrt(dx * dx + dy * dy);
    spine.segments[i].angle = atan2(dy, dx);

    if (i > 0) {
      // float angleDiff = normalizeAngle(body[i - 1].angle - body[i -
      // 2].angle);
      float angleDiff =
          normalizeAngle(spine.segments[i].angle - spine.segments[i - 1].angle);

      if (angleDiff > minAngle) {
        // body[i - 1].angle = normalizeAngle(body[i - 2].angle + minAngle);
        // body[i - 1].angle =
        spine.segments[i].angle =
            normalizeAngle(spine.segments[i - 1].angle + minAngle);
      } else if (angleDiff < -minAngle) {
        // body[i - 1].angle =
        spine.segments[i].angle =
            normalizeAngle(spine.segments[i - 1].angle - minAngle);
      }
    }

    spine.segments[i].end.x =
        spine.segments[i].start.x - cos(spine.segments[i].angle) * linkLength;
    spine.segments[i].end.y =
        spine.segments[i].start.y - sin(spine.segments[i].angle) * linkLength;

    spine.segments[i + 1].start = spine.segments[i].end;
  }
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
    body[i].drawPoints.left.x =
        body[i].pos[0] + body[i].size * cos(body[i].angle + M_PI / 2);
    body[i].drawPoints.left.y =
        body[i].pos[1] + body[i].size * sin(body[i].angle + M_PI / 2);
    body[i].drawPoints.right.x =
        body[i].pos[0] + body[i].size * cos(body[i].angle - M_PI / 2);
    body[i].drawPoints.right.y =
        body[i].pos[1] + body[i].size * sin(body[i].angle - M_PI / 2);
  }

  // Legs
  for (int i = 0; i < 4; i++) {
    legs[i].drawPoints[0].left.x =
        legs[i].pos[0].x + 7 * cos(legs[i].angles[0] + M_PI / 2);
    legs[i].drawPoints[0].left.y =
        legs[i].pos[0].y + 7 * sin(legs[i].angles[0] + M_PI / 2);
    legs[i].drawPoints[0].right.x =
        legs[i].pos[0].x - 7 * cos(legs[i].angles[0] + M_PI / 2);
    legs[i].drawPoints[0].right.y =
        legs[i].pos[0].y - 7 * sin(legs[i].angles[0] + M_PI / 2);
    legs[i].drawPoints[1].left.x =
        legs[i].pos[1].x + 7 * cos(legs[i].angles[0] + M_PI / 2);
    legs[i].drawPoints[1].left.y =
        legs[i].pos[1].y + 7 * sin(legs[i].angles[0] + M_PI / 2);
    legs[i].drawPoints[1].right.x =
        legs[i].pos[1].x - 7 * cos(legs[i].angles[0] + M_PI / 2);
    legs[i].drawPoints[1].right.y =
        legs[i].pos[1].y - 7 * sin(legs[i].angles[0] + M_PI / 2);
    legs[i].drawPoints[2].left.x =
        legs[i].pos[1].x + 7 * cos(legs[i].angles[1] + M_PI / 2);
    legs[i].drawPoints[2].left.y =
        legs[i].pos[1].y + 7 * sin(legs[i].angles[1] + M_PI / 2);
    legs[i].drawPoints[2].right.x =
        legs[i].pos[1].x - 7 * cos(legs[i].angles[1] + M_PI / 2);
    legs[i].drawPoints[2].right.y =
        legs[i].pos[1].y - 7 * sin(legs[i].angles[1] + M_PI / 2);
    legs[i].drawPoints[3].left.x =
        legs[i].pos[2].x + 7 * cos(legs[i].angles[1] + M_PI / 2);
    legs[i].drawPoints[3].left.y =
        legs[i].pos[2].y + 7 * sin(legs[i].angles[1] + M_PI / 2);
    legs[i].drawPoints[3].right.x =
        legs[i].pos[2].x - 7 * cos(legs[i].angles[1] + M_PI / 2);
    legs[i].drawPoints[3].right.y =
        legs[i].pos[2].y - 7 * sin(legs[i].angles[1] + M_PI / 2);
  }

  // for (int i = 0; i < 4; i++) {
  //   for (int j = 0; j < 4; j++) {
  //     int posIndex = (j < 2) ? 0 : ((j == 2) ? 1 : 2);
  //     int angleIndex = (j < 2) ? 0 : 1;
  //
  //     double angle = legs[i].angles[angleIndex] + M_PI / 2;
  //     double offsetX = 7 * cos(angle);
  //     double offsetY = 7 * sin(angle);
  //
  //     legs[i].drawPoints[j].left.x = legs[i].pos[posIndex].x + offsetX;
  //     legs[i].drawPoints[j].left.y = legs[i].pos[posIndex].y + offsetY;
  //     legs[i].drawPoints[j].right.x = legs[i].pos[posIndex].x - offsetX;
  //     legs[i].drawPoints[j].right.y = legs[i].pos[posIndex].y - offsetY;
  //   }
  // }
}

void Animal::drawLinks(UI *ui) {
  // Connect draw points on each side
  for (int i = 0; i < sizeof(body) / sizeof(body[0]) - 1; i++) {
    // SDL_RenderDrawLine(ui->getRenderer(), drawPoints[i].left.x,
    //                    drawPoints[i].left.y, drawPoints[i + 1].left.x,
    //                    drawPoints[i + 1].left.y);
    // SDL_RenderDrawLine(ui->getRenderer(), drawPoints[i].right.x,
    //                    drawPoints[i].right.y, drawPoints[i + 1].right.x,
    //                    drawPoints[i + 1].right.y);

    // SDL_RenderDrawLine(ui->getRenderer(), body[i].drawPoints.left.x,
    //                    body[i].drawPoints.left.y, body[i +
    //                    1].drawPoints.left.x, body[i + 1].drawPoints.left.y);
    // SDL_RenderDrawLine(ui->getRenderer(), body[i].drawPoints.right.x,
    //                    body[i].drawPoints.right.y,
    //                    body[i + 1].drawPoints.right.x,
    //                    body[i + 1].drawPoints.right.y);
    ui->DrawCircle(body[i].pos[0], body[i].pos[1], body[i].size);
  }

  // Head
  // arcRGBA(ui->getRenderer(), body[0].pos[0], body[0].pos[1], body[0].size,
  //         (body[0].angle - M_PI / 2) / M_PI * 180.0,
  //         (body[0].angle + M_PI / 2) / M_PI * 180.0, 249, 245, 215, 255);
  ui->DrawArc({body[0].pos[0], body[0].pos[1]}, body[0].size,
              body[0].angle - M_PI / 2, body[0].angle + M_PI / 2);

  // Eyes
  ui->DrawCircle(
      body[0].pos[0] + body[0].size / 2 * cos(body[0].angle + M_PI / 3),
      body[0].pos[1] + body[0].size / 2 * sin(body[0].angle + M_PI / 3), 4);
  ui->DrawCircle(
      body[0].pos[0] + body[0].size / 2 * cos(body[0].angle - M_PI / 3),
      body[0].pos[1] + body[0].size / 2 * sin(body[0].angle - M_PI / 3), 4);
}

const Leg &Animal::getLeg(int index) const {
  if (index < 0 || index >= 4) {
    throw std::out_of_range("Index out of bounds");
  }
  return legs[index];
}

const Spine &Animal::getSpine() const { return spine; }
