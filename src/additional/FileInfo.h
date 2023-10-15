#pragma once

#include <fstream>

class FileInfo {
 private:
  static const std::string infoFilename_m;

 public:
  FileInfo() = default;

  static bool fileEmpty(const std::string& filename) {
    bool res;
    std::ifstream in(filename, std::fstream::binary | std::fstream::in);
    if (!in.read(reinterpret_cast<char*>(&res), sizeof(res)))
      return true;
    else {
      in.close();
      std::ofstream out(infoFilename_m,
                        std::fstream::binary | std::fstream::out);
    }
  }
};