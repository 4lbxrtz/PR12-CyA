#pragma once

#include <iostream>
#include <vector>
#include <utility>

namespace cya {

typedef std::pair<double, double> Point;
typedef std::pair<Point, Point> Line;
typedef std::vector<Point> PointVector;

enum Side {
  kLeft = -1,
  kCenter = 0,
  kRight = 1
};

class PointSet : public PointVector {
 public:
  PointSet(const std::vector<Point>& points);

  void QuickHull();

  void WriteHull(std::ostream& os) const;
  void Write(std::ostream& os) const;

  inline const PointVector& GetHull() const { return hull_; }
  inline const PointVector& GetPoints() const { return *this;}
  
  void QuickHull(const Line& line, int side);
  void QuickHullUpgrade(const Line& line, int side); // 
  // todo: QuickHullBestStart

  double Distance(const Line& line, const Point& point) const;
  int FindSide(const Line& line, const Point& point) const;
  void XBounds(Point& min_x, Point& max_x) const;
  double PointToLine(const Line& line, const Point& point) const;
  bool FarthestPoint(const Line& line, int side, Point& farthest) const;

 private:
  PointVector hull_;
};

}  // namespace cya
