#include <iostream>
#include <ostream>
#include <sstream>
/*
 * Curiously Recrring Template Pattern(CRTP) with Mixins
 **/

// Base class - Point
template <template <typename> typename... Features>
class Point : public Features<Point<Features...>>... {
public:
  double x, y;

  Point() : Features<Point>()..., x(0), y(0) {}
  Point(double x, double y, Features<Point>... features)
      : Features<Point>(features)... {}

  std::string str() const {
    std::stringstream ss;
    ss << "[ x = " << x << ", y = " << y << " ] ";
    (ss << ... << Features<Point>::str());
    return ss.str();
  };
};

// Feature class - Color
template <typename Point> struct Color {
  short r, g, b;

  Color() : r(0), g(0), b(0) {}
  Color(short r, short g, short b) : r(r), g(g), b(b) {}

  std::string str() const {
    std::stringstream ss;
    ss << "[ r = " << r << ", g = " << g << ", b = " << b << " ] ";
    return ss.str();
  };
};

// Feature class - transparancy
template <typename Point> struct Transparancy {
  double transparancy;

  Transparancy() : transparancy(0){};
  Transparancy(double trans) : transparancy(trans){};

  std::string str() const {
    std::stringstream ss;
    ss << "[ transparancy = " << transparancy << " ] ";
    return ss.str();
  };
};

// Helper function to print Point
template <template <typename> typename... Features>
std::ostream &operator<<(std::ostream &os, const Point<Features...> &point) {
  return os << point.str();
}

int main(int argc, char *argv[]) {
  Point<> native_point(1.0, 5.0);
  Point<Color> colored_point(1.0, 5.0, {255, 255, 0});
  Point<Transparancy> transparant_point(1.0, 5.0, 0.8);

  Point<Color, Transparancy> colored_transparant_point(1.0, 5.0, {255, 255, 0},
                                                       0.8);

  std::cout << " native point: " << native_point << std::endl;
  std::cout << " colored point: " << colored_point << std::endl;
  std::cout << " transparant point: " << transparant_point << std::endl;
  std::cout << " colored transparant point: " << colored_transparant_point
            << std::endl;

  return 0;
}
