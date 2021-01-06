#include <cmath>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace basics {

int answer() {
  return 42;
}

int sum(int a, int b) {
  return a + b;
}

double midpoint(double left, double right){
  return left + (right - left)/2;
}

double weighted_midpoint(double left, double right, double alpha=0.5) {
  return left + (right - left) * alpha;
}

struct Point {

  Point() : Point(0, 0) {}
  Point(double x, double y) : x(x), y(y) {}

  static const Point origin;

  double length() const {
    return std::sqrt(x*x + y*y);
  }

  double distance_to(double other_x, double other_y) const {
    double dx = x - other_x;
    double dy = y - other_y;
    return std::sqrt(dx*dx + dy*dy);
  }

  double distance_to(const Point& other) const {
    return distance_to(other.x, other.y);
  }

  double x, y;
};

const Point Point::origin = Point(0, 0);

}

void bind_basics(py::module& basics) {

  using namespace basics;

  // Functions
  basics.def("answer", &answer);
  basics.def("sum", &sum);
  basics.def("midpoint", &midpoint, py::arg("left"), py::arg("right"));
  basics.def("weighted_midpoint", weighted_midpoint, py::arg("left"), py::arg("right"), py::arg("alpha")=0.5);


  // Classes
  py::class_<Point> pyPoint(basics, "Point");

  pyPoint
    .def(py::init<>())
    .def(py::init<double, double>(), py::arg("x"), py::arg("y"))
    .def("distance_to", py::overload_cast<double, double>(&Point::distance_to, py::const_), py::arg("x"), py::arg("y"))
    .def("distance_to", py::overload_cast<const Point&>(&Point::distance_to, py::const_), py::arg("other"))
    .def_readwrite("x", &Point::x)
    .def_readwrite("y", &Point::y)
    .def_property_readonly("length", &Point::length)
  ;

  pyPoint.attr("origin") = Point::origin;

  // Module-level attributes
  basics.attr("PI") = std::acos(-1);
  basics.attr("__version__") = "0.0.1";
}

PYBIND11_MODULE(python_example, m) {

  auto basics = m.def_submodule("basics");
  bind_basics(basics);

}