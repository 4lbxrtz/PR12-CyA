#include <algorithm>
#include <cmath>

#include "point_set.h"

namespace cya {

PointSet::PointSet(const std::vector<Point>& points) : PointVector(points) {}

void PointSet::QuickHull() {
  hull_.clear();

  Point min_x_point; 
  Point max_x_point;

  XBounds(min_x_point, max_x_point);

  QuickHull(Line(min_x_point, max_x_point), kLeft);
  QuickHull(Line(min_x_point, max_x_point), kRight);

  // Remove duplicates
  std::sort(hull_.begin(), hull_.end());
  hull_.erase(std::unique(hull_.begin(), hull_.end()), hull_.end());
}

void PointSet::XBounds(Point& min_x, Point& max_x) const {
  min_x = *std::min_element(begin(), end(), [](const Point& a, const Point& b) {
    return a.first < b.first;
  });
  max_x = *std::max_element(begin(), end(), [](const Point& a, const Point& b) {
    return a.first < b.first;
  });
}

void PointSet::QuickHull(const Line& line, int side) {
  // Implementation of the QuickHull algorithm
}

void PointSet::QuickHullUpgrade(const Line& line, int side) {
  // Implementation of the optimized QuickHull algorithm
}



bool PointSet::FarthestPoint(const Line& line, int side, Point& farthest) const {
  farthest = PointVector::at(0);
  double max_dist = 0;
  bool found = false;
  for (const Point& point : *this) {
    const double dist = Distance(line, point);

    if (FindSide(line, point) == side && dist > max_dist) {
      farthest = point;
      max_dist = dist;
      found = true;
    }
  }
  return found;
}

double PointSet::PointToLine(const Line& line, const Point& point) const {
  const Point& p1 = line.first;
  const Point& p2 = line.second;
  return (point.second - p1.second) * (p2.first - p1.first) -
         (p2.second - p1.second) * (point.first - p1.first);
}
double PointSet::Distance(const Line& line, const Point& point) const {
  return std::fabs(PointToLine(line, point));
}

int PointSet::FindSide(const Line& line, const Point& point) const {
  double value = PointToLine(line, point);
  if (value > 0) {
    return kLeft;
  } else if (value < 0) {
    return kRight;
  } else {
    return kCenter;
  }
}

void PointSet::Write(std::ostream& os) const {
  for (const Point& point : *this) {
    os << point.first << " " << point.second
        << std::endl;
  }
}

void PointSet::WriteHull(std::ostream& os) const {
  for (const Point& point : hull_) {
    os << point.first << " " << point.second
        << std::endl;
  }
}

}  // namespace cya