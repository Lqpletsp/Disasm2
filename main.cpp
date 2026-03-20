#include "executor.h"
#include "lexer.h"
#include "tokenizer.h"

int main() {
  std::string Code = R"(
    out 12 min "Hello";
    )";

  TokenGrid tokens = Tokenizer(Code);
  TokenGrid_t labeledtoken = CreateLabeledTokenTable(tokens);
  CheckSyntax(labeledtoken);
  ExecuteMainCode(labeledtoken);
  return 0;
}
