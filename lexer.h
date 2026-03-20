#pragma once
#include "types.h"

extern const std::vector<std::string> Commands; // is located in lexer.cpp
TokenGrid_t CreateLabeledTokenTable(const TokenGrid &TokenizedLines);
void CheckSyntax(const TokenGrid_t &labeledtoken);
bool FindCMD(const std::string &token);
bool ValidateName(const std::string &Name);
