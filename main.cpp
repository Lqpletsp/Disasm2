#include "executor.h"
#include "lexer.h"
#include "tokenizer.h"

int main() {
  std::string Code = R"(
  decm 20; 
  decv -ad Nums*10; 
    )";

  TokenGrid tokens = Tokenizer(Code);
  TokenGrid_t labeledtoken = CreateLabeledTokenTable(tokens);
  CheckSyntax(labeledtoken);
  DeclareIdentifiersAndMemory(labeledtoken);
  ExecuteMainCode(labeledtoken);
  return 0;
}
