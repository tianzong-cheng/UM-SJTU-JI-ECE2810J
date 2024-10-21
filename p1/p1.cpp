#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

struct Point {
  int x, y;
  bool operator<(const Point &p) const {
    return x < p.x || (x == p.x && y < p.y);
  }
};

double ccw(const Point &a, const Point &b, const Point &c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

struct Compare {
  Point p0;
  Compare(const Point &p0) : p0(p0) {}
  bool operator()(const Point &a, const Point &b) const {
    double t = ccw(p0, a, b);
    return t > 0 || (t == 0 && a.y < b.y);
  }
};

int main() {
  int n;
  std::cin >> n;

  if (n == 0) {
    return 0;
  }

  std::set<Point> point_set;
  for (int i = 0; i < n; ++i) {
    Point p;
    std::cin >> p.x >> p.y;
    point_set.insert(p);
  }

  std::vector<Point> points(point_set.begin(), point_set.end());

  int index = 0;
  for (int i = 1; i < points.size(); ++i) {
    if (points[i].y < points[index].y ||
        (points[i].y == points[index].y && points[i].x < points[index].x)) {
      index = i;
    }
  }

  std::swap(points[0], points[index]);

  std::sort(points.begin() + 1, points.end(), Compare(points[0]));

  std::vector<Point> hull;
  for (auto &p : points) {
    while (hull.size() >= 2 &&
           ccw(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0) {
      hull.pop_back();
    }
    hull.push_back(p);
  }

  for (auto &p : hull) {
    std::cout << p.x << ' ' << p.y << std::endl;
  }
}
