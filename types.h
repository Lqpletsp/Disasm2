#pragma once
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

struct token {
  std::string Type;
  std::string TokenName;
};

struct MemSlot {
  std::string type;
  std::string value;
  bool occupied = false;
};

struct Variable {
  std::string name;
  std::string type;
  std::vector<int> slots;
};

inline std::vector<MemSlot> g_pool;
inline std::stack<int> g_freeSlots;
inline std::unordered_map<std::string, Variable> g_vars;
inline bool g_memDeclared = false;

using Line = std::vector<std::string>;
using TokenGrid = std::vector<Line>;
using Line_t = std::vector<token>;
using TokenGrid_t = std::vector<Line_t>;

struct CodeState { // used for outputting error
  int CurrentLine = -1;
  int TokenIndex = -1;
  Line_t CurrentTokens;
  std::string CurrentFile = "main";
};

extern CodeState CurrentState;

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
