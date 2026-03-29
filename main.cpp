#include "executor.h"
#include "lexer.h"
#include "tokenizer.h"

int main() {
  std::string Code = R"(
  decm 50;
  dec var. -d Num; 
  set 10: Num; 
  out Num -n "hello world" -n; 
    )";

  TokenGrid tokens = Tokenizer(Code);
  TokenGrid_t labeledtoken = CreateLabeledTokenTable(tokens);
  ExecuteMainCode(labeledtoken);
  return 0;
}
