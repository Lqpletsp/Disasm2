#include "tokenizer.h"

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
      if (ch == '"') {
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
      if (ch == '*' || ch == '@' || ch == '~' || ch == ',')
        // ',' for error outputting
        currentLine.push_back(std::string(1, ch));
      currentToken.clear();
    } else {
      currentToken += ch;
    }
  }
  return result;
}
