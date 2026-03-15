// NOTE: FOR ANY WEIRD OR UNECESSARY INDENTATIONS, ASTRONVIM IS TO BE BLAMED :) 
// This is currently just a tokenizer
#include <iostream>
#include <string>
#include <vector>

const std::vector<std::string> Commands = {"out", "in", "decm", "decv"};

struct token {
  std::string Type;
  std::string TokenName;
};

using Token = std::string;
using Line = std::vector<Token>;
using TokenGrid = std::vector<Line>;
using Token_t = token;               // type token
using Line_t = std::vector<Token_t>; // vector to store token types
using TokenGrid_t =
    std::vector<Line_t>; // vector to store vector to store token types

void printStuff(const TokenGrid_t &lines) { // debug function
  for (const Line_t &line : lines) {
    std::cout << "[";
    for (const Token_t &eachtoken : line) {
      std::cout << "[" << eachtoken.Type << "," << eachtoken.TokenName << "]"
                << ",";
    }
    std::cout << "]\n";
  }
}

void CheckSyntax(const TokenGrid_t &labeledtoken);
void DeclareIdentifiers();
void ExecuteCode();
TokenGrid_t CreateLabeledTokenTable(const TokenGrid &TokenizedLines);
TokenGrid Tokenizer(const std::string &codelines);
bool FindCMD(const std::string &token);
void printStuff(const TokenGrid &TokenizedLines); // debug function
bool ValidateName(const std::string &Name);
bool isdigit(const std::string &str);
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

int main() {
  std::string Code = R"(
  out -;  
  )";

  TokenGrid tokens = Tokenizer(Code);
  TokenGrid_t labeledtoken = CreateLabeledTokenTable(tokens);
  printStuff(labeledtoken);
  CheckSyntax(labeledtoken);
  return 0;
}
void CheckSyntax(const TokenGrid_t &labeledtoken) {
  for (int LineIndex = 0; LineIndex < labeledtoken.size(); LineIndex++) {
    Line_t Line = labeledtoken.at(LineIndex);
    if (Line.at(0).Type != "cmd") {
      std::cout
          << "ERR[" << "ln:" << LineIndex << "|col:" << "0" // Just a debug line. Will be changed
          << "] : Beginning of each line must be a valid command. None found";
      return;
    }
    for (int TokenIndex = 1; TokenIndex < Line.size(); TokenIndex++) {
      token CurrentToken = Line.at(TokenIndex);
      if (CurrentToken.Type == "???") {
        std::cout << "ERR[" << "ln:" << LineIndex << "|col:" << TokenIndex // Just a debug line. Will be changed
                  << "] : Invalid token given. Cannot intepret '"
                  << CurrentToken.TokenName << "'";
      }
    }
  }
}

TokenGrid Tokenizer(const std::string &codelines) {
  TokenGrid result;
  Line currentLine;
  Token currentToken;
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

          if (tokenStr.at(0) == '"' && tokenStr.back() == '"')
            token_struct.Type = "str"; // string
          else if (ValidateName(line.at(TokenIndex)))
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
