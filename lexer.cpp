
#include "lexer.h"
#include "declarators.h"
#include "errors.h"
#include "types.h"
#include <string>

const std::vector<std::string> Commands = {"out",  "decm", "dec",   ".add",
                                           ".min", ".div", ".mult", "set",
                                           ".var", ".all", ".fnc",  "end"};

TokenGrid_t CreateLabeledTokenTable(const TokenGrid &TokenizedLines) {
  TokenGrid_t LabelledTokenTable;
  for (size_t LineIndex = 0; LineIndex < TokenizedLines.size(); LineIndex++) {
    Line line = TokenizedLines.at(LineIndex);
    Line_t Labelledline;
    bool asi = false;
    token asiMetadata;
    token token_struct;
    std::string linecmd;
    CurrentState.CurrentLine += 1;
    CurrentState.TokenIndex = 0;
    if (!FindCMD(line.at(0)))
      OutError("Byte Code creation Failed -> Every line must begin with a "
               "command, none found.");
    else if (line.at(0) == "decm") {
      if (line.at(0) == "decm" && (line.size() > 2 || line.size() < 2))
        OutError("Byte Code creation Failed -> decm takes 1 argument of type "
                 "digit but was not given.");
      try {
        std::stoi(line.at(1));
        token_struct.Type = "!!!"; // completed, can ignore
        token_struct.TokenName = "decm";
        Labelledline.clear();
        Labelledline.push_back(token_struct);
        LabelledTokenTable.push_back(Labelledline);
        DecmC(std::stoi(line.at(1)));
        continue;
      } catch (...) {
        OutError("Byte Code creation Failed -> decm takes 1 argument of type "
                 "digit but was not given.");
      }
    } else {
      token_struct.TokenName = line.at(0);
      token_struct.Type = "cmd";
      linecmd = line.at(0);
      Labelledline.push_back(token_struct);
    }
    for (size_t TokenIndex = 1; TokenIndex < line.size(); TokenIndex++) {
      CurrentState.TokenIndex += 1;
      std::string tokenStr = line.at(TokenIndex);
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
            OutError("Byte Code creation Failed -> Garbage token given-- Could "
                     "not interpret");
        }
      }
      Labelledline.push_back(token_struct);
    }
    if (asi) {
      if (linecmd != "set")
        OutError("Byte Code creation Failed -> ':' can only be used by the set "
                 "command");
      Labelledline.push_back(asiMetadata);
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
