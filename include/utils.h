#pragma once

#include <string>
#include <optional>

#include "point_set.h"



namespace utils {
struct Arguments {
  std::string input_file;
  std::string output_file;
  bool dot;
  bool benchmark;
  bool upgrade_method;
  std::optional<cya::Point> point;
};

Arguments ParseArguments(const std::vector<std::string>& arguments);
std::string Help();
cya::PointSet ReadPoints(const std::string& file_name);
void BenchmarkEMST(const cya::PointSet& point_set);
void WriteEMST(const cya::PointSet& point_set, const std::string& file_name, bool dot);
void Run(int argc, char* argv[]);
}