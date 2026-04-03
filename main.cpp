#include "executor.h"
#include "lexer.h"
#include "tokenizer.h"

int main() {
  std::string Code = R"(
  decm 50;
  dec .fnc MyFnc;
  out "Hello world";
  dec .fnc MyFnc2; 
  end .fnc; 
  end .fnc;
  out "Hello world";
    )";

  TokenGrid tokens = Tokenizer(Code);
  TokenGrid_t labeledtoken = CreateLabeledTokenTable(tokens);
  ExecuteMainCode(labeledtoken);
  return 0;
}
