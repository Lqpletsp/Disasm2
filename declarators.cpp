#include "declarators.h"
#include "errors.h"
#include "types.h"
void DecmC(const int &size);
void DecvC(const Line_t &Tokens);

void HandleDecC(const Line_t &Tokens);

void DecmC(const int &size) {
  CurrentState.TokenIndex += 1;
  if (g_memDeclared) {
    OutError("decm error. Memory already declared.");
    return;
  }
  g_pool.resize(size);
  for (int i = size - 1; i >= 0; i--)
    g_freeSlots.push(i);
  g_memDeclared = true;
}
void DecvC(const Line_t &Tokens) {
  std::string DataType;
  bool array = false;
  int count = 1; // can change for arrays when implemented
  if (!g_memDeclared) {
    OutError("Memory not declared. Cannot declare variables.");
  }
  for (size_t TokenIndex = 0; TokenIndex < Tokens.size(); TokenIndex++) {
    CurrentState.TokenIndex += 1;
    token LineToken = Tokens.at(TokenIndex);
    if (array && LineToken.TokenName ==
                     "*") // array declaration format: <identifier>*<size>. If
                          // array is noticed then it handles * and <size>
                          // immediately so need to skip it
                          // this also means that you need to repeatedly mention
                          // if the identifier is array or not

      continue;
    else if (array && LineToken.Type == "dig") {
      array = false;
      continue;
    }
    if (LineToken.Type == "mmd") {
      for (const char instruction : LineToken.TokenName) {
        switch (instruction) {
        case '-':
          break;
        case 'd':
          DataType = "dig"; // digit (could be float or int)
          break;
        case 's':
          DataType = "str"; // string
          break;
        case 'b':
          DataType = "bol"; // boolean
          break;
        case 'a':
          array = true;

          break;
        case 'v':
          array = false;
        default:
          OutError("Invalid data type given during variable declaration. "
                   "Cannot interpret");
        }
      }
    } else if (LineToken.Type == "var") {
      if (array) {
        try {
          if (Tokens.at(TokenIndex + 2).Type == "dig")
            count = std::stoi(Tokens.at(TokenIndex + 2).TokenName);
        } catch (...) {
          OutError("Not enough arguments given to declare an array identifier");
        }
      }
      if ((int)g_freeSlots.size() < count) {
        OutError("decm error. Ran out of memory");
      }

      Variable VariableToken;
      VariableToken.name = LineToken.TokenName;
      VariableToken.type = DataType;
      for (int i = 0; i < count; i++) {
        if (g_freeSlots.empty())
          OutError("decm error. Ran out of memory");
        int index = g_freeSlots.top();
        g_freeSlots.pop();
        g_pool[index].occupied = true;
        g_pool[index].type = DataType;
        g_pool[index].value = "";
        VariableToken.slots.push_back(index);
      }
      g_vars[LineToken.TokenName] = VariableToken;

    } else {
      OutError("Garbage data given for variable declaration. \n decv command "
               "only takes data type(s) and variable name(s). ");
    }
  }
}

void DecfC(const Line_t &Tokens) {
  Function FuncitonDetails;
  Variable ParameterVariable;
  InsideFunction = true;
  CurrentState.TokenIndex += 1;
  if (Tokens.at(0).Type == "var") {
    if (!InsideFunction) {
      FuncitonDetails.name = Tokens.at(0).TokenName;
      FuncitonDetails.FunctionStartLine = CurrentState.CurrentLine;
      g_functions[Tokens.at(0).TokenName] = FuncitonDetails;
      LoadedFunctions.push(FuncitonDetails.name);
      for (size_t TokenIndex = 1; TokenIndex < Tokens.size(); TokenIndex++) {
        token param = Tokens.at(TokenIndex);
        Variable NewVar;
        if (param.Type != "var")
          OutError(
              "Function declaration only allows identifiers as parameters");
        NewVar.type = "uni"; // unidentified
        NewVar.name = param.TokenName;
        // slots is also not determined
        FuncitonDetails.Parameters.push_back(NewVar);
      }
    } else
      LoadedFunctions.push(Tokens.at(0).TokenName);
  } else
    OutError("Invalid name given for function during declaration");
}

void HandleDecC(const Line_t &Tokens) {
  CurrentState.TokenIndex += 1;
  if (Tokens.at(0).Type != "cmd")
    OutError("dec command must be followed by idenfier declarators (var., "
             "fnc.,loop., label.) but none were found");
  Line_t SlicedLine = SliceStuff(1, Tokens.size() - 1, Tokens);
  if (Tokens.at(0).TokenName == ".var" && !InsideFunction)
    DecvC(SlicedLine);
  else if (Tokens.at(0).TokenName == ".fnc")
    DecfC(SlicedLine);
  else
    OutError("dec command must be followed by idenfier declarators (var., "
             "fnc., loop., label.) but none were found");
}
