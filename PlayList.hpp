#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct PlayList {
  std::vector<std::string> files;
  std::vector<std::string> titles;

  void load(const fs::path &directory);
  size_t size() const;
};