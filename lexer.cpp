
#include "lexer.h"
#include "errors.h"
#include "types.h"
#include <iostream>
#include <string>

const std::vector<std::string> Commands = {"out", "decm", "decv", "add",
                                           "min", "div",  "mult", "set"};

TokenGrid_t CreateLabeledTokenTable(const TokenGrid &TokenizedLines) {
  TokenGrid_t LabelledTokenTable;
  for (Line line : TokenizedLines) {
    Line_t Labelledline;
    bool asi = false;
    token asiMetadata;

    for (size_t TokenIndex = 0; TokenIndex < line.size(); TokenIndex++) {
      std::string tokenStr = line.at(TokenIndex);
      token token_struct;
      token_struct.TokenName = tokenStr;
      if (FindCMD(tokenStr))
        token_struct.Type = "cmd";
      else {
        try {
          std::stoi(tokenStr);
          token_struct.Type = "dig";
        } catch (...) {
          if (tokenStr.at(0) == '"' && tokenStr.back() == '"') {
            token_struct.Type = "str";
            token_struct.TokenName = tokenStr;
          } else if (tokenStr.size() == 1 &&
                     (tokenStr == "T" || tokenStr == "F"))
            token_struct.Type = "bol";
          else if (ValidateName(line.at(TokenIndex)))
            token_struct.Type = "var";
          else if (tokenStr == "*" || tokenStr == "@")
            token_struct.Type = "opr";
          else if (tokenStr.at(0) == '-')
            token_struct.Type = "mmd";
          else if (tokenStr == "]")
            token_struct.Type = "stp";
          else if (tokenStr == "," || tokenStr == " ")
            token_struct.Type = "ign";
          else if (tokenStr == ":") {
            token_struct.Type = "asi"; // assigning operator
            asiMetadata.Type = "None";
            asiMetadata.TokenName = std::to_string(TokenIndex);
            asi = true;
          } else
            token_struct.Type = "???";
        }
      }
      Labelledline.push_back(token_struct);
    }
    if (asi)
      Labelledline.push_back(asiMetadata);
    LabelledTokenTable.push_back(Labelledline);
  }
  return LabelledTokenTable;
}

bool FindCMD(const std::string &token) {
  for (const std::string &cmd : Commands)
    if (token == cmd)
      return true;
  return false;
}

bool ValidateName(const std::string &Name) {
  std::string validchars =
                  "abcdefghilmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_",
              nums = "1234567890";
  bool found = false, alphanumericFound = false;
  for (char ch : Name) {
    found = SearchCh(ch, validchars);
    if (!found && !alphanumericFound) {
      found = SearchCh(ch, nums);
      if (found)
        alphanumericFound = true;
      else if (!found)
        return false;
    } else if (found && alphanumericFound)
      return false;
  }
  return true;
}
void CheckSyntax(const TokenGrid_t &labeledtoken) {
  for (size_t LineIndex = 0; LineIndex < labeledtoken.size(); LineIndex++) {
    CurrentState.CurrentLine += 1;
    CurrentState.TokenIndex = 0;
    Line_t Line = labeledtoken.at(LineIndex);
    CurrentState.CurrentTokens = Line;
    token cmd = Line.at(0);
    if (cmd.Type != "cmd") {
      OutError("Each line must begin with a command, none found.");
      return;
    } else if (cmd.TokenName == "decm") {
      if (Line.size() > 2 || Line.size() < 2) {

        OutError(
            "decm command takes no more or less than 1 argument of type "
            "digit\n"
            "If there are operators as argument(s), those are not allowed");
      } else if (Line.at(1).Type == "str" || Line.at(1).Type == "var") {
        OutError("decm command takes one argument of type digit.");
      }
    }
    for (size_t TokenIndex = 1; TokenIndex < Line.size(); TokenIndex++) {
      CurrentState.TokenIndex += 1;
      token CurrentToken = Line.at(TokenIndex);
      if (CurrentToken.Type == "???") {
        OutError("Garbage token given. Cannot interpret");
      } else if (CurrentToken.Type == "None" && cmd.TokenName != "set") {
        CurrentState.TokenIndex = std::stoi(CurrentToken.TokenName);
        OutError("The assigning operator, ':', can only be used with set "
                 "command.\nGarbage token given");
      }
    }
  }
}
