#include "PlayList.hpp"
#include <filesystem>

namespace fs = std::filesystem;

void PlayList::load(const fs::path &dicrectory) {
  files.clear();
  titles.clear();

  fs::directory_options opts = fs::directory_options::skip_permission_denied;

  for (const auto &entry : fs::recursive_directory_iterator(dicrectory, opts)) {
    if (entry.path().extension() == ".flac") {
      files.push_back(entry.path().string());
      titles.push_back(entry.path().stem().string());
    }
  }
}

size_t PlayList::size() const { return files.size(); }