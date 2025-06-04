#include "Parser.hpp"
#include <string>
#include <memory>

int main(void){
    std::shared_ptr<Lexer> lex = std::make_shared<Lexer>();
    std::shared_ptr<Parser> prs = std::make_shared<Parser>();
    std::string line = "";
    debugParsing(prs->initParsing(lex->lexing(line)));
}