#pragma once

#include <chrono>
#include <functional>
#include <iostream>

struct Profiler {
  static decltype(std::chrono::high_resolution_clock::now()) bv, ev;
  template <typename C, typename... Args>
  static void coutTime(const std::string& str, C&& func, Args&&... args) {
    bv = std::chrono::high_resolution_clock::now();
    std::bind(std::forward<C>(func), std::forward<Args>(args)...);
    ev = std::chrono::high_resolution_clock::now();
    std::cout << str
              << std::chrono::duration_cast<std::chrono::microseconds>(ev - bv)
                     .count()
              << std::endl;
  }
  static void begin() { bv = std::chrono::high_resolution_clock::now(); }
  static void end(const std::string& str) {
    ev = std::chrono::high_resolution_clock::now();
    std::cout << str
              << std::chrono::duration_cast<std::chrono::microseconds>(ev - bv)
                     .count()
              << std::endl;
  }
};