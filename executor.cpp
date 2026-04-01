// executor
#include "executor.h"
#include "declarators.h"
#include "errors.h"
#include "types.h"
#include <iostream>

double BridgeFncRtr(const Line_t &Tokens);
double AddC(const Line_t &Tokens);
double MinC(const Line_t &Tokens);
double MultC(const Line_t &Tokens);
double DivC(const Line_t &Tokens);
void OutF(const Line_t &Tokens);
void SetC(const Line_t &Tokens);
void InpC(const Line_t &Tokens);

void ExecuteMainCode(const TokenGrid_t &labeledtoken) {
  for (size_t LineIndex = 0; LineIndex < labeledtoken.size(); LineIndex++) {
    CurrentState.CurrentLine = LineIndex;
    CurrentState.TokenIndex = 0;
    CurrentState.CurrentTokens = labeledtoken.at(LineIndex);

    Line_t Line = labeledtoken.at(LineIndex);
    if (Line.empty() || Line.at(0).Type == "!!!")
      continue;

    std::string cmd = Line.at(0).TokenName;
    Line_t SlicedLine = SliceStuff(1, Line.size() - 1, Line);

    if (cmd == "out")
      OutF(SlicedLine);
    else if (cmd == "set")
      SetC(SlicedLine);
    else if (cmd == "inp")
      InpC(SlicedLine);
    else if (cmd == "dec")
      HandleDecC(SlicedLine);
  }
}
void InpC(const Line_t &Tokens) { return; }

double BridgeFncRtr(const Line_t &Tokens) {
  // Tokens[0] is the command name, rest are its arguments
  std::string cmd = Tokens.at(0).TokenName;
  Line_t args = SliceStuff(1, Tokens.size() - 1, Tokens);

  // Increment TokenIndex to point past the command name
  CurrentState.TokenIndex += 1;

  if (cmd == "add.")
    return AddC(args);
  else if (cmd == "min.")
    return MinC(args);
  else if (cmd == "mult.")
    return MultC(args);
  else if (cmd == "div.")
    return DivC(args);
  else {
    OutError("Cannot use void commands inside other commands");
    return 0;
  }
}

double AddC(const Line_t &Tokens) {
  double AddedTotal = 0;
  bool InsideCommand = false;

  for (size_t i = 0; i < Tokens.size(); i++) {
    CurrentState.TokenIndex += 1;
    token CurrentToken = Tokens.at(i);

    if (InsideCommand && CurrentToken.Type == "cmd")
      break;
    else if (InsideCommand && CurrentToken.Type == "stp") {
      InsideCommand = false;
      continue;
    }

    if (!InsideCommand) {
      if (CurrentToken.Type == "dig") {
        AddedTotal += std::stod(CurrentToken.TokenName);
      } else if (CurrentToken.Type == "cmd") {
        InsideCommand = true;
        AddedTotal += BridgeFncRtr(SliceStuff(i, Tokens.size() - 1, Tokens));
      } else if (CurrentToken.Type == "stp") {
        break;
      } else if (CurrentToken.Type == "ign") {
        continue;
      } else if (CurrentToken.Type == "str") {
        OutError("Cannot use string data type for addition");
      }
    }
  }
  return AddedTotal;
}

double MinC(const Line_t &Tokens) {
  double Subtracted = 0;
  bool InsideCommand = false;

  token first = Tokens.at(0);
  CurrentState.TokenIndex += 1;

  if (first.Type == "cmd") {
    Subtracted = BridgeFncRtr(SliceStuff(0, Tokens.size() - 1, Tokens));
    InsideCommand = true;
  } else if (first.Type == "dig") {
    Subtracted = std::stod(first.TokenName);
  } else if (first.Type == "ign") {
    Subtracted = 0;
  } else if (first.Type == "str") {
    OutError("Cannot use string data type for subtraction");
  }

  for (size_t i = 1; i < Tokens.size(); i++) {
    CurrentState.TokenIndex += 1;
    token CurrentToken = Tokens.at(i);

    if (InsideCommand && CurrentToken.Type == "cmd")
      break;
    else if (InsideCommand && CurrentToken.Type == "stp") {
      InsideCommand = false;
      continue;
    }

    if (!InsideCommand) {
      if (CurrentToken.Type == "dig") {
        Subtracted -= std::stod(CurrentToken.TokenName);
      } else if (CurrentToken.Type == "cmd") {
        InsideCommand = true;
        Subtracted -= BridgeFncRtr(SliceStuff(i, Tokens.size() - 1, Tokens));
      } else if (CurrentToken.Type == "stp") {
        break;
      } else if (CurrentToken.Type == "ign") {
        continue;
      } else if (CurrentToken.Type == "str") {
        OutError("Cannot use string data type for subtraction");
      }
    }
  }
  return Subtracted;
}

double DivC(const Line_t &Tokens) {
  double Divided = 0;
  bool InsideCommand = false;

  token first = Tokens.at(0);
  CurrentState.TokenIndex += 1;

  if (first.Type == "cmd") {
    Divided = BridgeFncRtr(SliceStuff(0, Tokens.size() - 1, Tokens));
    InsideCommand = true;
  } else if (first.Type == "dig") {
    Divided = std::stod(first.TokenName);
  } else if (first.Type == "ign") {
    Divided = 0;
  } else {
    OutError("Cannot use string data type for division");
  }

  for (size_t i = 1; i < Tokens.size(); i++) {
    CurrentState.TokenIndex += 1;
    token CurrentToken = Tokens.at(i);

    if (InsideCommand && CurrentToken.Type == "cmd")
      break;
    else if (InsideCommand && CurrentToken.Type == "stp") {
      InsideCommand = false;
      continue;
    }

    if (!InsideCommand) {
      if (CurrentToken.Type == "dig") {
        Divided /= std::stod(CurrentToken.TokenName);
      } else if (CurrentToken.Type == "cmd") {
        InsideCommand = true;
        Divided /= BridgeFncRtr(SliceStuff(i, Tokens.size() - 1, Tokens));
      } else if (CurrentToken.Type == "stp") {
        break;
      } else if (CurrentToken.Type == "str") {
        OutError("Cannot use string data type for division");
      }
    }
  }
  return Divided;
}

double MultC(const Line_t &Tokens) {
  double Multiplied = 1;
  bool InsideCommand = false;

  token first = Tokens.at(0);
  CurrentState.TokenIndex += 1;

  if (first.Type == "cmd") {
    Multiplied = BridgeFncRtr(SliceStuff(0, Tokens.size() - 1, Tokens));
    InsideCommand = true;
  } else if (first.Type == "dig") {
    Multiplied = std::stod(first.TokenName);
  } else if (first.Type == "ign") {
    Multiplied = 0;
  } else {
    OutError("Cannot use string data type for multiplication");
  }

  for (size_t i = 1; i < Tokens.size(); i++) {
    CurrentState.TokenIndex += 1;
    token CurrentToken = Tokens.at(i);

    if (InsideCommand && CurrentToken.Type == "cmd")
      break;
    else if (InsideCommand && CurrentToken.Type == "stp") {
      InsideCommand = false;
      continue;
    }

    if (!InsideCommand) {
      if (CurrentToken.Type == "dig") {
        Multiplied *= std::stod(CurrentToken.TokenName);
      } else if (CurrentToken.Type == "cmd") {
        InsideCommand = true;
        Multiplied *= BridgeFncRtr(SliceStuff(i, Tokens.size() - 1, Tokens));
      } else if (CurrentToken.Type == "stp") {
        break;
      } else if (CurrentToken.Type == "str") {
        OutError("Cannot use string data type for multiplication");
      }
    }
  }
  return Multiplied;
}

BundleData SearchVariables(const std::string &name) {
  BundleData Variable;
  if (!g_vars.count(name))
    OutError("Undeclared variable,'" + name + "', used");
  Variable.found = true;
  Variable.data = g_pool[g_vars[name].slots[0]].value;
  Variable.dataType = g_pool[g_vars[name].slots[0]].type;
  return Variable;
}
void StoreDataInVariable(const std::string &name, const std::string &data) {
  if (!g_vars.count(name))
    OutError("Undeclared variable,'" + name + "', used for assignment");
  g_pool[g_vars[name].slots[0]].value = data;
}
void SetC(const Line_t &Tokens) {
  CurrentState.TokenIndex += 1;
  BundleData RawDataToStore;

  if (Tokens.back().Type != "None") {
    OutError("set command requires assigning operator but was not given.");
  }

  int asioperatorindex = std::stoi(Tokens.back().TokenName);

  if (asioperatorindex < 1 || (size_t)asioperatorindex > Tokens.size() - 2) {
    CurrentState.TokenIndex = asioperatorindex;
    OutError("Assigning operator misplaced for set command\n"
             "Assignment has format: set <data> : <identifier>");
  }

  // ---- get the data side ----
  if (Tokens.at(0).Type != "cmd") {
    Line_t DataToStore = SliceStuff(0, asioperatorindex - 2, Tokens);
    if (DataToStore.size() != 1)
      OutError("Cannot store more or less than one data in variable(s)");
    Line_t().swap(DataToStore);

    if (Tokens.at(0).Type == "var") {
      BundleData SearchedVariable = SearchVariables(Tokens.at(0).TokenName);
      if (!SearchedVariable.found)
        OutError("Undeclared variable,'" + Tokens.at(0).TokenName +
                 "' used for assignment");
      RawDataToStore.data = SearchedVariable.data;
      RawDataToStore.dataType = SearchedVariable.dataType;
    } else {
      RawDataToStore.data = Tokens.at(0).TokenName;
      RawDataToStore.dataType = Tokens.at(0).Type;
    }
  } else {
    RawDataToStore.data = std::to_string(
        BridgeFncRtr(SliceStuff(0, asioperatorindex - 2, Tokens)));
    RawDataToStore.dataType = "dig";
  }

  // ---- assign to variable(s) ----
  for (size_t TokenIndex = asioperatorindex; TokenIndex < Tokens.size() - 1;
       TokenIndex++) {
    CurrentState.TokenIndex += 1;
    token CurrentVariable = Tokens.at(TokenIndex);

    if (CurrentVariable.Type == "ign")
      continue;

    if (CurrentVariable.Type != "var") {
      OutError("Garbage token given after assignment operator.\n"
               "Only identifiers can be used to store data");
    }

    BundleData VariableToStoreIn = SearchVariables(CurrentVariable.TokenName);
    if (VariableToStoreIn.dataType != RawDataToStore.dataType)
      OutError(
          "Assignment error. Data type mismatch between data and identifier");

    StoreDataInVariable(CurrentVariable.TokenName, RawDataToStore.data);
  }
}
void OutF(const Line_t &Tokens) {
  for (size_t i = 0; i < Tokens.size(); i++) {
    CurrentState.TokenIndex += 1;
    token CurrentToken = Tokens.at(i);
    std::string DataToPrint;
    std::string extra = "";
    if (CurrentToken.Type == "str")
      DataToPrint = SliceStuff(1, CurrentToken.TokenName.size() - 2,
                               CurrentToken.TokenName);
    else if (CurrentToken.Type == "dig" || CurrentToken.Type == "bol")
      DataToPrint = CurrentToken.TokenName;
    else if (CurrentToken.Type == "var") {
      BundleData Variable = SearchVariables(CurrentToken.TokenName);
      if (Variable.dataType == "dig" || Variable.dataType == "bol") {
        DataToPrint = Variable.data;
      } else {
        // slice Variable.data, not CurrentToken.TokenName
        DataToPrint = SliceStuff(1, Variable.data.size() - 2, Variable.data);
      }
    } else if (CurrentToken.Type == "cmd")
      DataToPrint = BridgeFncRtr(SliceStuff(i, Tokens.size() - 1, Tokens));
    else if (CurrentToken.Type == "mmd") {
      for (const char mmd : CurrentToken.TokenName) {
        switch (mmd) {
        case '-':
          break;
        case 'n':
          extra = "\n";
          break;
        default:
          OutError("Garbage mid line command given for out command");
        }
      }
    }
    std::cout << DataToPrint + extra;
  }
}
