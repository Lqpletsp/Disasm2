#include "executor.h"
#include "lexer.h"
#include "tokenizer.h"

int main() {
  std::string Code = R"(
    decm 100; 
    decv -s Name -d Age -b Student; 
    set "James" : Name; 
    set 20 : Age; 
    set T : Student;
    out Name,"|",Age,"|",Student;
    )";

  TokenGrid tokens = Tokenizer(Code);
  TokenGrid_t labeledtoken = CreateLabeledTokenTable(tokens);
  CheckSyntax(labeledtoken);
  DeclareIdentifiersAndMemory(labeledtoken);
  ExecuteMainCode(labeledtoken);
  return 0;
}
