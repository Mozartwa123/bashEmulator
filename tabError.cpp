#include "tokenizer.hpp"
#include <string>

int main(void){
    /*
    std::shared_ptr<Lexer> lex = std::make_shared<Lexer>();
    std::shared_ptr<Parser> prs = std::make_shared<Parser>();
    std::string line = "";
    debugParsing(prs->initParsing(lex->lexing(line)));
    */
    debugSplit(pathSplitLast("/etc"));
    debugSplit(pathSplitLast("/etc/dev"));
    debugSplit(pathSplitLast("~/.local/share/applications"));
}