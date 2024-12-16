#include "utils.h"
#include "point_set.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <optional>

namespace utils {

Arguments ParseArguments(const std::vector<std::string>& arguments) {
  bool dot = false;
  bool benchmark = false;
  bool upgrade_method = false;
  std::optional<cya::Point> point;
  if (arguments.size() < 3 || arguments.size() > 10) {
    std::cerr << "Error: invalid number of arguments" << std::endl;
    std::cerr << Help() << std::endl;
    exit(EXIT_FAILURE);
  }
  for (const std::string& argument : arguments) {
    if (argument == "-h" || argument == "--help") {
      std::cout << Help() << std::endl;
      exit(EXIT_SUCCESS);
    }
  }
  const std::string& input_file = arguments[1];
  const std::string& output_file = arguments[2];
  for (const auto& argument : arguments) {
    if (argument == "-d") {
      dot = true;
    }
    if (argument == "-b") {
      benchmark = true;
    }
    if (argument == "-u") {
      upgrade_method = true;
    }
    if (argument == "--inside") {
      std::stringstream ss(arguments[arguments.size() - 2] + " " + arguments[arguments.size() - 1]);
      double x, y;
      ss >> x >> y;
      point = cya::Point(x, y);
    }
    if (input_file == output_file) {
      std::cerr << "Error: input and output files must be different" << std::endl;
      std::cerr << Help() << std::endl;
      exit(EXIT_FAILURE);
    }
    if (input_file.empty() || output_file.empty()) {
      std::cerr << "Error: invalid input/output file" << std::endl;
      std::cerr << Help() << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  return Arguments{input_file, output_file, dot, benchmark, upgrade_method, point};
}

std::string Help() {
  std::stringstream ss;
  ss << "Usage: ./cya_p12 <input_file> <output_file>" << std::endl;
  ss << "Options:" << std::endl;
  ss << "  -h, --help  Show this help message and exit" << std::endl;
  ss << "input_file: file with the set of points" << std::endl;
  ss << "output_file: file with the convex hull" << std::endl;
  return ss.str();
}

cya::PointSet ReadPoints(const std::string& file_name) {
  std::ifstream is(file_name);
  if (!is.is_open()) {
    std::cerr << "Error: unable to open input file" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::vector<cya::Point> points;
  int num_points;
  is >> num_points;
  
  for (int i = 0; i < num_points; ++i) {
    double x, y;
    is >> x >> y;
    points.emplace_back(x, y);
  }
  return cya::PointSet(points);
}

void BenchmarkQuickHull(cya::PointSet point_set) {
  using namespace std::chrono;

  const int num_executions = 30;
  long long total_duration_quick_hull = 0;
  long long total_duration_quick_hull_upgrade = 0;

  for (int i = 0; i < num_executions; ++i) {
    // Benchmark original QuickHull
    auto start = high_resolution_clock::now();
    point_set.ComputeQuickHull();
    auto end = high_resolution_clock::now();
    total_duration_quick_hull += duration_cast<microseconds>(end - start).count();

    // Benchmark optimized QuickHullUpgrade
    start = high_resolution_clock::now();
    point_set.ComputeQuickHullUpgrade();
    end = high_resolution_clock::now();
    total_duration_quick_hull_upgrade += duration_cast<microseconds>(end - start).count();
  }

  long long mean_duration_quick_hull = total_duration_quick_hull / num_executions;
  long long mean_duration_quick_hull_upgrade = total_duration_quick_hull_upgrade / num_executions;

  // Output results
  std::cout << "Benchmark Results (averaged over " << num_executions << " executions):" << std::endl;
  std::cout << "Original QuickHull Time: " << mean_duration_quick_hull << " microseconds" << std::endl;
  std::cout << "Upgraded QuickHull Time: " << mean_duration_quick_hull_upgrade << " microseconds" << std::endl;

  if (mean_duration_quick_hull_upgrade < mean_duration_quick_hull) {
    long long improvement = mean_duration_quick_hull - mean_duration_quick_hull_upgrade;
    double improvement_percentage = (static_cast<double>(improvement) / mean_duration_quick_hull) * 100;
    std::cout << "Optimized method is faster by " << improvement << " microseconds (" << improvement_percentage << "%)." << std::endl;
  } else if (mean_duration_quick_hull_upgrade > mean_duration_quick_hull) {
    long long slowdown = mean_duration_quick_hull_upgrade - mean_duration_quick_hull;
    double slowdown_percentage = (static_cast<double>(slowdown) / mean_duration_quick_hull) * 100;
    std::cout << "Original method is faster by " << slowdown << " microseconds (" << slowdown_percentage << "%)." << std::endl;
  } else {
    std::cout << "Both methods have identical performance." << std::endl;
  }
}

void WriteHull(const cya::PointSet& point_set, const std::string& file_name, bool dot) {
  std::ofstream os(file_name);
  if (!os.is_open()) {
    std::cerr << "Error: unable to open output file" << std::endl;
    exit(EXIT_FAILURE);
  }
  if (dot) {
    point_set.WriteDOT(os);
  } else {
    point_set.Write(os);
  }
}

void Run(int argc, char* argv[]) {
  std::vector<std::string> arguments(argv, argv + argc);
  auto parsed_arguments = ParseArguments(arguments);
  cya::PointSet point_vector = ReadPoints(parsed_arguments.input_file);
  if (parsed_arguments.benchmark) {
    BenchmarkQuickHull(point_vector);
    return;
  }
  if (parsed_arguments.upgrade_method) {
    std::cout << "Using upgraded QuickHull method." << std::endl;
    point_vector.ComputeQuickHullUpgrade();
  } else {
    std::cout << "Using original QuickHull method." << std::endl;
    point_vector.ComputeBestConvexHull();
  }
  if (parsed_arguments.point.has_value()) {
    const cya::Point& point = parsed_arguments.point.value();
    if (point_vector.IsPointHull(point)) {
      std::cout << "The point (" << point.first << ", " << point.second << ") is in the hull." << std::endl;
    } else {
      std::cout << "The point (" << point.first << ", " << point.second << ") is not in the hull." << std::endl;
    }
    return;
  }
  WriteHull(point_vector, parsed_arguments.output_file, parsed_arguments.dot);
}

}  // namespace utils