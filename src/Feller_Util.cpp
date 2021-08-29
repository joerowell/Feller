#include "Feller_Util.hpp"

std::string
Feller::Util::time_to_string(const std::chrono::time_point<std::chrono::system_clock> &time)
{
  // This is a remarkably involved process for some reason.
  // The neatest way I found of doing this was found at:
  // https://gist.github.com/polaris/adee936198995a6f8c697c419d21f734
  std::time_t t  = std::chrono::system_clock::to_time_t(time);
  std::string ts = std::ctime(&t);
  ts.resize(ts.size() - 1);
  return ts;
}
