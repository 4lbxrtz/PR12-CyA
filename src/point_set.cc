#include <algorithm>
#include <cmath>
#include <iostream>

#include "point_set.h"

namespace cya {



void PointSet::ComputeQuickHull() {
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

void PointSet::ComputeQuickHullUpgrade() {
  hull_.clear();

  Point min_x_point; 
  Point max_x_point;

  XBounds(min_x_point, max_x_point);

  hull_.push_back(min_x_point);
  hull_.push_back(max_x_point);

  QuickHullUpgrade(Line(min_x_point, max_x_point), kLeft);
  QuickHullUpgrade(Line(min_x_point, max_x_point), kRight);

  // Remove duplicates
  std::sort(hull_.begin(), hull_.end());
  hull_.erase(std::unique(hull_.begin(), hull_.end()), hull_.end());
}

void PointSet::XBounds(Point& min_x, Point& max_x) const {
  if (empty()) {
    std::cerr << "Error: PointSet is empty" << std::endl;
    exit(EXIT_FAILURE);
  }

  min_x = *std::min_element(begin(), end(), [](const Point& a, const Point& b) {
    return a.first < b.first;
  });
  max_x = *std::max_element(begin(), end(), [](const Point& a, const Point& b) {
    return a.first < b.first;
  });
}

void PointSet::QuickHull(const cya::Line& line, int side) {
  cya::Point farthest;
  if (FarthestPoint(line, side, farthest)) {
    QuickHull(cya::Line(line.first, farthest), -FindSide(cya::Line(line.first, farthest), line.second));
    QuickHull(cya::Line(farthest, line.second), -FindSide(cya::Line(farthest, line.second), line.first));
  } else {
    hull_.push_back(line.first);
    hull_.push_back(line.second);
  }
}

void PointSet::QuickHullUpgrade(const Line& line, int side) {
  Point farthest;
  if (!FarthestPoint(line, side, farthest)) {
    return;
  }
  hull_.push_back(farthest);
  Line line1 = {line.first, farthest};
  Line line2 = {farthest, line.second};
  QuickHullUpgrade(line1, -FindSide(line1, line.second));
  QuickHullUpgrade(line2, -FindSide(line2, line.first));
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
  double dx = line.second.first - line.first.first;
  double dy = line.second.second - line.first.second;
  return std::abs(dy * point.first - dx * point.second + line.second.first * line.first.second - line.second.second * line.first.first) /
         std::sqrt(dx * dx + dy * dy);
}

int PointSet::FindSide(const Line& line, const Point& point) const {
  double val = (line.second.second - line.first.second) * (point.first - line.first.first) -
               (line.second.first - line.first.first) * (point.second - line.first.second);
  if (val > 0) {
    return 1;
  } else if (val < 0) {
    return -1;
  } else {
    return 0;
  }
}

void PointSet::Write(std::ostream& os) const {
  for (const Point& point : hull_) {
    os << "(" << point.first << ", " << point.second << ")"
        << std::endl;
  }
}

void PointSet::WriteDOT(std::ostream& os) const {
  for (const Point& point : hull_) {
    os << point.first << " " << point.second
        << std::endl;
  }
}

}  // namespace cya