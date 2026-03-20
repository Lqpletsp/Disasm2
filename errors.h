#pragma once
#include "types.h"
#include <string>

void OutError(const Line_t &ErrLine, const int &LineIndexOrg,
              const int &TokenIndexOrg, const std::string ErrMsg);
