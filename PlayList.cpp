#include "PlayList.hpp"
#include <filesystem>

namespace fs = std::filesystem;

void PlayList::load(const fs::path &directory) {
  files.clear();
  titles.clear();
  fs::directory_options opts = fs::directory_options::skip_permission_denied;

  for (const auto &entry : fs::recursive_directory_iterator(directory, opts)) {
    if (entry.path().extension() == ".flac") {
      files.emplace_back(entry.path());
      titles.emplace_back(entry.path().stem().native());
    }
  }
}

size_t PlayList::size() const { return files.size(); }