#pragma once

#include "Globals.h"

struct FirstOpen {
  static const std::string saveFilename_m;
  FirstOpen() {
    std::fstream stream(saveFilename_m,
                        std::fstream::binary | std::fstream::in);
    if (empty(stream))
      gameFirstOpen = true;
    else
      gameFirstOpen = false;
  }
  ~FirstOpen() {
    std::fstream stream(saveFilename_m,
                        std::fstream::binary | std::fstream::out);
    bool tmp = true;
    stream.write(reinterpret_cast<char*>(&tmp), sizeof(tmp));
  }
};