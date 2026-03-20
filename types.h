#pragma once
#include <string>
#include <vector>

struct token {
  std::string Type;
  std::string TokenName;
};

using Line = std::vector<std::string>;
using TokenGrid = std::vector<Line>;
using Line_t = std::vector<token>;
using TokenGrid_t = std::vector<Line_t>;

template <typename T, typename U>
bool SearchCh(const T &ToSearch, const U &DataToSearchFrom) {
  for (const T TS : DataToSearchFrom)
    if (TS == ToSearch)
      return true;
  return false;
}

template <typename T>
T SliceStuff(const int &Start, const int &End, const T &DataToSlice) {
  T SlicedT;
  for (int i = Start; i <= End; i++)
    SlicedT.push_back(DataToSlice.at(i));
  return SlicedT;
}
