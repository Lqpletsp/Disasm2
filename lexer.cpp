#include "lexer.h"
#include "errors.h"

const std::vector<std::string> Commands = {"out", "in",  "decm", "decv",
                                           "add", "min", "div",  "mult"};

TokenGrid_t CreateLabeledTokenTable(const TokenGrid &TokenizedLines) {
  TokenGrid_t LabelledTokenTable;
  for (Line line : TokenizedLines) {
    Line_t Labelledline;
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
          } else if (ValidateName(line.at(TokenIndex)))
            token_struct.Type = "var";
          else if (tokenStr == "*" || tokenStr == "@")
            token_struct.Type = "opr";
          else if (tokenStr.at(0) == '!')
            token_struct.Type = "mmd";
          else if (tokenStr == "~")
            token_struct.Type = "stp";
          else
            token_struct.Type = "???";
        }
      }
      Labelledline.push_back(token_struct);
    }
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
  std::string validchars = "abcdefghilmnopqrstuvwxyz_", nums = "1234567890";
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
    Line_t Line = labeledtoken.at(LineIndex);
    if (Line.at(0).Type != "cmd") {
      OutError(Line, LineIndex, 0,
               "Each line must begin with a command, none found.");
      return;
    }
    for (size_t TokenIndex = 1; TokenIndex < Line.size(); TokenIndex++) {
      token CurrentToken = Line.at(TokenIndex);
      if (CurrentToken.Type == "???") {
        OutError(Line, LineIndex, TokenIndex,
                 "Garbage token given. Cannot interpret");
      }
    }
  }
}
