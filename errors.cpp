#include "errors.h"
#include <cstdlib>
#include <iostream>

void OutError(const Line_t &ErrLine, const int &LineIndexOrg,
              const int &TokenIndexOrg, const std::string ErrMsg) {
  std::string ErrPointer = "";
  for (size_t TokenIndex = 0; TokenIndex < ErrLine.size(); TokenIndex++) {
    std::cout << ErrLine.at(TokenIndex).TokenName << ' ';
    if (TokenIndex == (size_t)TokenIndexOrg)
      ErrPointer += std::string(ErrLine.at(TokenIndex).TokenName.size(), '^');
    else
      ErrPointer +=
          std::string(ErrLine.at(TokenIndex).TokenName.size() + 1, ' ');
  }
  std::cout << "\n\033[1;31m" << ErrPointer;
  std::cout << "\033[1;31m" << "\nERR[" << LineIndexOrg << "|" << TokenIndexOrg
            << "] : " << ErrMsg << "\033[0m" << '\n';
  exit(1);
}
