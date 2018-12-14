#ifndef POOL_UTILS_H_
#define POOL_UTILS_H_

#include <string>
#include <sstream>
#include <vector>


class Strings {
public:
  static std::string Format(const char * fmt, ...);
  static void Append(std::string & dest, const char * fmt, ...);
};

#endif
