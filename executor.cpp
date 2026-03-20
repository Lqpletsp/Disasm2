#include "executor.h"
#include "errors.h"
#include <iostream>

double BridgeFncRtr(const Line_t &SlicedTokens, const int &TokenIndex,
                    const int &LineIndex, const std::string &cmd,
                    const Line_t &AllTokens);
double AddC(const int TokenIndexOrg, const Line_t &Tokens, const int &LineIndex,
            const Line_t &AllTokens);
double MinC(const int TokenIndexOrg, const Line_t &Tokens, const int &LineIndex,
            const Line_t &AllTokens);
double MultC(const int TokenIndexOrg, const Line_t &Tokens,
             const int &LineIndex, const Line_t &AllTokens);
double DivC(const int TokenIndexOrg, const Line_t &Tokens, const int &LineIndex,
            const Line_t &AllTokens);
void OutF(const Line_t &LineTokens, const int &RowLine,
          const Line_t &AllTokens);

void ExecuteMainCode(const TokenGrid_t &labeledtoken) {
  Line_t SlicedLine;
  for (size_t LineIndex = 0; LineIndex < labeledtoken.size(); LineIndex++) {
    Line_t Line = labeledtoken.at(LineIndex);
    if (Line.empty()) {
      continue;
    }
    std::string token = Line.at(0).TokenName;
    SlicedLine = SliceStuff(1, Line.size() - 1, Line);
    if (token == "out") {
      OutF(SlicedLine, LineIndex, Line);
    }
  }
}

double BridgeFncRtr(const Line_t &SlicedTokens, const int &TokenIndex,
                    const int &LineIndex, const std::string &cmd,
                    const Line_t &AllTokens) {
  if (cmd == "add")
    return AddC(TokenIndex, SlicedTokens, LineIndex, AllTokens);
  else if (cmd == "min") {
    return MinC(TokenIndex, SlicedTokens, LineIndex, AllTokens);
  } else {
    OutError(AllTokens, LineIndex, TokenIndex,
             "Cannot use void commands inside other commands");
    return 0;
  }
}
double AddC(const int TokenIndexOrg, const Line_t &Tokens, const int &LineIndex,
            const Line_t &AllTokens) {
  double AddedTotal = 0;
  token CurrentToken;
  bool InsideCommand = false;
  for (size_t TokenIndex = 0; TokenIndex < Tokens.size(); TokenIndex++) {
    CurrentToken = Tokens.at(TokenIndex);
    if (InsideCommand && CurrentToken.Type == "cmd") {
      break;
    } else if (InsideCommand && CurrentToken.Type == "stp") {
      InsideCommand = false;
      continue;
    }
    if (!InsideCommand) {
      if (CurrentToken.Type == "dig") {
        AddedTotal += std::stod(CurrentToken.TokenName);
      } else if (CurrentToken.Type == "cmd") {
        InsideCommand = true;
        AddedTotal +=
            BridgeFncRtr(SliceStuff(TokenIndex + 1, Tokens.size() - 1, Tokens),
                         TokenIndex + TokenIndexOrg, LineIndex,
                         CurrentToken.TokenName, AllTokens);
      } else if (CurrentToken.Type == "stp") {
        break;
      } else if (CurrentToken.Type == "str") {
        OutError(
            Tokens, LineIndex, TokenIndex + TokenIndexOrg + 1,
            "Cannot use string data type for addition"); // or var but variables
                                                         // are not supported
                                                         // right now
      }
    }
  }
  return AddedTotal;
}
double MinC(const int TokenIndexOrg, const Line_t &Tokens, const int &LineIndex,
            const Line_t &AllTokens) {

  token CurrentToken;
  bool InsideCommand = false;
  double Subtracted;

  if (Tokens.at(0).Type == "cmd") {
    Subtracted =
        BridgeFncRtr(SliceStuff(1, Tokens.size() - 1, Tokens), TokenIndexOrg,
                     LineIndex, Tokens.at(0).TokenName, AllTokens);
    InsideCommand = true;
  }

  else if (Tokens.at(0).Type == "dig")
    Subtracted = std::stod(Tokens.at(0).TokenName);
  else if (Tokens.at(0).Type == "str")
    OutError(AllTokens, LineIndex, TokenIndexOrg + 1,
             "Cannot use string data type for subtraction");

  for (size_t TokenIndex = 1; TokenIndex < Tokens.size(); TokenIndex++) {
    CurrentToken = Tokens.at(TokenIndex);
    if (InsideCommand && CurrentToken.Type == "cmd") {
      break;
    } else if (InsideCommand && CurrentToken.Type == "stp") {
      InsideCommand = false;
      continue;
    }
    if (!InsideCommand) {
      if (CurrentToken.Type == "dig") {
        Subtracted -= std::stod(CurrentToken.TokenName);
      } else if (CurrentToken.Type == "cmd") {
        InsideCommand = true;
        Subtracted -= BridgeFncRtr(
            SliceStuff(TokenIndex + 1, Tokens.size() - 1, Tokens), TokenIndex,
            LineIndex, CurrentToken.TokenName, AllTokens);
      } else if (CurrentToken.Type == "stp") {
        break;
      } else if (CurrentToken.Type == "str") {
        OutError(
            AllTokens, LineIndex, TokenIndex + TokenIndexOrg + 1,
            "Cannot use string data type for subtraction"); // or var but
                                                            // variables are not
                                                            // supported right
                                                            // now
      }
    }
  }
  return Subtracted;
}
double DivC(const int TokenIndexOrg, const Line_t &Tokens, const int &LineIndex,
            const Line_t &AllTokens) {

  token CurrentToken;
  bool InsideCommand = false;
  double Divided;
  if (Tokens.at(0).Type == "cmd") {
    Divided = BridgeFncRtr(SliceStuff(1, Tokens.size() - 1, Tokens),
                           TokenIndexOrg + 1, LineIndex, Tokens.at(0).TokenName,
                           AllTokens);
    InsideCommand = true;
  }

  else if (Tokens.at(0).Type == "dig")
    Divided = std::stod(Tokens.at(0).TokenName);
  else
    OutError(Tokens, LineIndex, 0, "Cannot use string data type for division");

  for (size_t TokenIndex = 1; TokenIndex < Tokens.size(); TokenIndex++) {
    CurrentToken = Tokens.at(TokenIndex);
    if (InsideCommand && CurrentToken.Type == "cmd") {
      break;
    } else if (InsideCommand && CurrentToken.Type == "stp") {
      InsideCommand = false;
      continue;
    }
    if (!InsideCommand) {
      if (CurrentToken.Type == "dig") {
        Divided -= std::stod(CurrentToken.TokenName);
      } else if (CurrentToken.Type == "cmd") {
        InsideCommand = true;
        Divided /= BridgeFncRtr(
            SliceStuff(TokenIndex + 1, Tokens.size() - 1, Tokens), TokenIndex,
            LineIndex, CurrentToken.TokenName, AllTokens);
      } else if (CurrentToken.Type == "stp") {
        break;
      } else if (CurrentToken.Type == "str") {
        OutError(AllTokens, LineIndex, TokenIndex + TokenIndexOrg + 1,
                 "Cannot use string data type for division"); // or var but
                                                              // variables are
                                                              // not supported
                                                              // right now
      }
    }
  }
  return Divided;
}
double MultC(const int TokenIndexOrg, const Line_t &Tokens,
             const int &LineIndex, const Line_t &AllTokens) {

  token CurrentToken;
  bool InsideCommand = false;
  double Multplied = 1;

  for (size_t TokenIndex = 1; TokenIndex < Tokens.size(); TokenIndex++) {
    CurrentToken = Tokens.at(TokenIndex);
    if (InsideCommand && CurrentToken.Type == "cmd") {
      break;
    } else if (InsideCommand && CurrentToken.Type == "stp") {
      InsideCommand = false;
      continue;
    }
    if (!InsideCommand) {
      if (CurrentToken.Type == "dig") {
        Multplied *= std::stod(CurrentToken.TokenName);
      } else if (CurrentToken.Type == "cmd") {
        InsideCommand = true;
        Multplied *= BridgeFncRtr(
            SliceStuff(TokenIndex + 1, Tokens.size() - 1, Tokens), TokenIndex,
            LineIndex, CurrentToken.TokenName, AllTokens);
      } else if (CurrentToken.Type == "stp") {
        break;
      } else if (CurrentToken.Type == "str") {
        OutError(AllTokens, LineIndex, TokenIndex + TokenIndexOrg + 1,
                 "Cannot use string data type for division"); // or var but
                                                              // variables are
                                                              // not supported
                                                              // right now
      }
    }
  }
  return Multplied;
}

void OutF(const Line_t &LineTokens, const int &RowLine,
          const Line_t &AllTokens) {
  token CurrentToken;
  for (size_t TokenIndex = 0; TokenIndex < LineTokens.size(); TokenIndex++) {
    CurrentToken = LineTokens.at(TokenIndex);
    if (CurrentToken.Type == "str" || CurrentToken.Type == "dig") {
      if (CurrentToken.Type == "str")
        std::cout << SliceStuff(1, CurrentToken.TokenName.size() - 2,
                                CurrentToken.TokenName)
                  << '\n';
      else
        std::cout << CurrentToken.TokenName << '\n';
    } else if (CurrentToken.Type == "cmd") {
      std::cout << BridgeFncRtr(SliceStuff(TokenIndex + 1,
                                           LineTokens.size() - 1, LineTokens),
                                TokenIndex + 1, RowLine, CurrentToken.TokenName,
                                AllTokens)
                << '\n';
      return;
    }
  }
}
