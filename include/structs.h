#ifndef VEC2_H
#define VEC2_H

#include <deque>
#include <memory>

struct Vec2 {
  double x;
  double y;
};

struct Segment {
  // std::shared_ptr<Vec2> start;
  // std::shared_ptr<Vec2> end;
  Vec2 start;
  Vec2 end;

  float length;
  float angle;
  float size;

  // Segment(std::shared_ptr<Vec2> start, std::shared_ptr<Vec2> end, float
  // length,
  //         float angle)
  //     : start(std::move(start)), end(std::move(end)), length(length),
  //       angle(angle) {}
};

// static void addSegment(std::deque<Segment> &segments,
//                        std::shared_ptr<Vec2> newEnd, float length,
//                        float angle) {
//   auto newStart = segments.empty() ? std::make_shared<Vec2>(Vec2{0, 0})
//                                    : segments.back().end;
//   segments.emplace_back(newStart, std::move(newEnd), length, angle);
// }

#endif // VEC2_H
