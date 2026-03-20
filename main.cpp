#include "executor.h"
#include "lexer.h"
#include "tokenizer.h"

int main() {
  std::string Code = R"(
    out 12 min "Hello";
    )";

  TokenGrid tokens = Tokenizer(Code); // tokenizes the code
  TokenGrid_t labeledtoken = CreateLabeledTokenTable(tokens); 
    // identifies each tokens (str: string, dig: digit, cmd: command, mmd: mid line commands, ???: unknown)
  CheckSyntax(labeledtoken); // checks if each line has a command in the beginning or not and checks the existence of ??? tokens
  ExecuteMainCode(labeledtoken); // executes the code. Identifer declaration not supported so it immediately jumps to this. 
  return 0;
}
