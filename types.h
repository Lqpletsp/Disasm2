// types.h
#pragma once
#include <queue>
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
struct Function {
  std::string name;
  std::vector<Variable> parameters;
  int FunctionStartLine;
};

inline std::vector<MemSlot> g_pool;
inline std::stack<int> g_freeSlots;
inline std::unordered_map<std::string, Variable> g_vars;
inline bool g_memDeclared = false;
inline std::unordered_map<std::string, Function> g_functions;
inline std::stack<std::string> LoadedFunctions;

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

inline CodeState CurrentState;

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

struct BundleData {
  bool found;
  std::string dataType;
  std::string data;
};
