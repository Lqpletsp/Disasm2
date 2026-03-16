// NOTE: FOR ANY WEIRD OR UNECESSARY INDENTATIONS, ASTRONVIM IS TO BE BLAMED :) 
// This is currently just a tokenizer
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

const std::vector<std::string> Commands = {"out", "in", "decm",
                                           "decv"}; // list of valid commands
struct token {
  std::string Type;
  std::string TokenName;
};

using Line = std::vector<std::string>;
using TokenGrid = std::vector<Line>; // type token
using Line_t = std::vector<token>;   // vector to store token types
using TokenGrid_t =
    std::vector<Line_t>; // vector to store vector to store token types

void printStuff(const TokenGrid_t &lines) { // debug function
  for (const Line_t &line : lines) {
    std::cout << "[";
    for (const token &eachtoken : line) {
      std::cout << "[" << eachtoken.Type << "," << eachtoken.TokenName << "]"
                << ",";
    }
    std::cout << "]\n";
  }
}
void OutError(const Line_t &ErrLine, const int &RowLine, const int &ColLine,
              const std::string ErrMsg);
void CheckSyntax(const TokenGrid_t &labeledtoken);
void DeclareIdentifiers();
void ExecuteMainCode(const TokenGrid_t &labeledtoken);
TokenGrid_t CreateLabeledTokenTable(const TokenGrid &TokenizedLines);
TokenGrid Tokenizer(const std::string &codelines);
bool FindCMD(const std::string &token);
void printStuff(const TokenGrid &TokenizedLines); // debug function
bool ValidateName(const std::string &Name);
bool isdigit(const std::string &str);
void OutF(const Line_t &LineTokens, int &RowLine);
template <typename T, typename U>
bool SearchCh(const T &ToSearch,
              const U &DataToSearchFrom) { // !! Limited working for now.
                                           // Validating Variable name uses it
  for (const T TS : DataToSearchFrom) {
    if (TS == ToSearch) {
      return true;
    }
  }
  return false;
}
template <typename T>
T SliceStuff(const int &Start, const int &End,
             const T &VectorToSlice) { // Enter a valid range
  T SlicedT;
  for (int TokenIndex = Start; TokenIndex <= End; TokenIndex++) {
    SlicedT.push_back(VectorToSlice.at(TokenIndex));
  }
  return SlicedT;
}

int main() {
  std::string Code = R"(
  out "Hi" ; 
  )";

  TokenGrid tokens = Tokenizer(Code);
  TokenGrid_t labeledtoken = CreateLabeledTokenTable(tokens);
  // printStuff(labeledtoken); // debug line
  CheckSyntax(labeledtoken);
  ExecuteMainCode(labeledtoken);
  return 0;
}
void ExecuteMainCode(const TokenGrid_t &labeledtoken) {
  Line_t SlicedLine;
  for (int LineIndex = 0; LineIndex < labeledtoken.size(); LineIndex++) {
    Line_t Line = labeledtoken.at(LineIndex);
    if (Line.empty()) {
      continue;
    }
    std::string token = Line.at(0).TokenName;
    SlicedLine = SliceStuff(1, Line.size() - 1, Line);
    if (token == "out") {
      OutF(SlicedLine, LineIndex);
    }
  }
}

void OutF(const Line_t &LineTokens, int &RowLine) {
  token CurrentToken;
  for (int TokenIndex = 0; TokenIndex < LineTokens.size(); TokenIndex++) {
    CurrentToken = LineTokens.at(TokenIndex);
    if (CurrentToken.Type == "str" || CurrentToken.Type == "dig") {
      std::cout << CurrentToken.TokenName;
    }
  }
}
void CheckSyntax(const TokenGrid_t &labeledtoken) {
  for (int LineIndex = 0; LineIndex < labeledtoken.size(); LineIndex++) {
    Line_t Line = labeledtoken.at(LineIndex);
    if (Line.at(0).Type != "cmd") {
      OutError(Line, LineIndex, 0,
               "Each line must begin with a command, none found.");
      return;
    }
    for (int TokenIndex = 1; TokenIndex < Line.size(); TokenIndex++) {
      token CurrentToken = Line.at(TokenIndex);
      if (CurrentToken.Type == "???") {
        OutError(Line, LineIndex, TokenIndex,
                 "Garbage token given. Cannot interpret");
      }
    }
  }
}

void OutError(const Line_t &ErrLine, const int &RowLine, const int &ColLine,
              const std::string ErrMsg) {

  std::string ErrPointer = " ", underline = "";
  for (int TokenIndex = 0; TokenIndex < ErrLine.size(); TokenIndex++) {
    std::cout << ErrLine.at(TokenIndex).TokenName << ' ';
    if (TokenIndex == ColLine) {
      ErrPointer +=
          std::string(ErrLine.at(TokenIndex).TokenName.size() / 2, ' ');
      ErrPointer.append("^");
    } else {
      ErrPointer += std::string(ErrLine.at(TokenIndex).TokenName.size(), ' ');
    }
  }
  std::cout << '\n' << ErrPointer;
  std::cout << "\033[1;31m" << "\nERR[" << RowLine << "|" << ColLine
            << "] : " << ErrMsg << "\033[0m" << '\n';
  exit(1);
}

TokenGrid Tokenizer(const std::string &codelines) {
  TokenGrid result;
  Line currentLine;
  std::string currentToken;
  bool inComment = false;
  bool inString = false;

  for (const char ch : codelines) {
    if (inComment) {
      if (ch == '|')
        inComment = false;
      continue;
    }
    if (inString) {
      currentToken += ch;
      if (ch == '"') { // closing quote
        inString = false;
        currentLine.push_back(currentToken);
        currentToken.clear();
      }
      continue;
    }
    if (ch == '|') {
      inComment = true;
    } else if (ch == '"') {
      inString = true;
      currentToken += ch;
    } else if (ch == ';') {
      if (!currentToken.empty())
        currentLine.push_back(currentToken);
      if (!currentLine.empty())
        result.push_back(currentLine);
      currentToken.clear();
      currentLine.clear();
    } else if (ch == ' ' || ch == ',' || ch == '\t' || ch == '\n' ||
               ch == '\r' || ch == '*' || ch == '@') {
      if (!currentToken.empty())
        currentLine.push_back(currentToken);
      if (ch == '*' || ch == '@')
        currentLine.push_back(std::string(1, ch));
      currentToken.clear();
    } else {
      currentToken += ch;
    }
  }

  return result;
}

TokenGrid_t CreateLabeledTokenTable(const TokenGrid &TokenizedLines) {
  TokenGrid_t LabelledTokenTable;
  for (Line line : TokenizedLines) {
    Line_t Labelledline;
    for (int TokenIndex = 0; TokenIndex < line.size(); TokenIndex++) {
      std::string tokenStr = line.at(TokenIndex);
      token token_struct;
      token_struct.TokenName = tokenStr;
      if (FindCMD(tokenStr))
        token_struct.Type = "cmd";
      else {
        try {
          std::stoi(tokenStr);
          token_struct.Type = "dig"; // digit
        } catch (...) {

          if (tokenStr.at(0) == '"' && tokenStr.back() == '"') {
            token_struct.Type = "str"; // string
            token_struct.TokenName =
                SliceStuff(1, tokenStr.size() - 2, tokenStr); // Removes ""
          } else if (ValidateName(line.at(TokenIndex)))
            token_struct.Type = "var"; // variable
          else if (tokenStr == "*" || tokenStr == "@")
            token_struct.Type = "opr"; // operator
          else if (tokenStr.at(0) == '!')
            token_struct.Type = "mmd"; // mid line command
          else
            token_struct.Type = "???"; // unknown
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
      else if (!found) {
        return false;
      }
    } else if (found && alphanumericFound)
      return false; // invalid name because
                    // <char><alphanumeric><char>
  }
  return true;
}
