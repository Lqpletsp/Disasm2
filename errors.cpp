#include "errors.h"
#include <cstdlib>
#include <iostream>

void OutError(const std::string ErrMsg) {
  std::string ErrPointer = "";
  Line_t &ErrLine = CurrentState.CurrentTokens;
  int TokenIndexOrg = CurrentState.TokenIndex;
  std::cout << "\n\n\033[1;31m"
            << "!ERR!"
               "\033[0m\n\n";
  for (size_t TokenIndex = 0; TokenIndex < ErrLine.size(); TokenIndex++) {
    if (ErrLine.at(TokenIndex).Type != "None")
      std::cout << ErrLine.at(TokenIndex).TokenName << ' ';
    if (TokenIndex == (size_t)TokenIndexOrg)
      ErrPointer += std::string(ErrLine.at(TokenIndex).TokenName.size(), '^');
    else
      ErrPointer +=
          std::string(ErrLine.at(TokenIndex).TokenName.size() + 1, ' ');
  }
  std::cout << "\n\033[1;31m" << ErrPointer;
  std::cout << "\033[1;31m" << "\nERR[" << CurrentState.CurrentLine << "@"
            << TokenIndexOrg << "@" << CurrentState.CurrentFile
            << "] : " << ErrMsg << "\033[0m" << '\n';
  exit(1);
}
