//Parser.cpp
#include <iostream>
#include <memory>
#include <vector>
#include "Parser.hpp"
#include "error.hpp"

/*
Uwaga na oznaczenia beg i end:
tokens[beg] = pierwszy token, który parsujemy
tokens[end] = ostatni token który parsujemy
Czyli for(int i = beg; i<end; i++) NIEPOPRAWNE (CHYBA, ŻE OSTATNI ARGUMENT MA 
SPECYFICZNE ZNACZENIE)!!!
for(int i = beg; i<=end; i++) POPRAWNE!!!
*/
void debugCommand(Ast command);
void debugArg(Ast potArg) {
  if (potArg->type != NodeType::Arg) {
    debugCommand(potArg);
  }
  if (auto *arg = std::get_if<std::string>(&potArg->value)) {
    std::cout << *arg;
  }
  std::cerr << "Error during debugging Arg\n";
}

bool bindingCompare(std::shared_ptr<Token> tkn1, std::shared_ptr<Token> tkn2){
    if(tkn1 == nullptr || tkn2 == nullptr){
        std::cerr<<"Empty token durring bindong comparation\n";
        updateErrorMessage("Parser: Empty token durring bindong comparation");
        return false;
    }
    if(tkn1->givePriority()!= tkn2->givePriority()){
        return tkn2->givePriority()>tkn1->givePriority();
    }
    if(tkn1->giveBindingType() == Binding::LEFT){
        return true;
    }
    return false;
}

int Parser::findStrongestOperatorIndex(
        const std::vector<std::shared_ptr<Token>>& tokens, 
        int beg, 
        int end) {
    std::shared_ptr<Token> maxPrioToken = tokens[beg];
    int index = beg;
    for (int i = beg + 1; i <= end; ++i) { /*
         W przypadku dwóch takich samych tokenów, np. Pipe o wiązaniu lewostronnym:
         pierwszy pipe zostanie zastąpiony drugim i stworzy się podział (a | b) | c
         Jeżeli mamy coś prawostronnego to pierwszy pozostanie i będzie podział:
         a ^ (b ^ c), gdzie ^ to spójkik prawostronny
        */
        if (bindingCompare(maxPrioToken, tokens[i])) {
            maxPrioToken = tokens[i];
            index = i;
        }
    }
    return index;
}

Ast Parser::parseCommand(const std::vector<std::shared_ptr<Token>>& tokens, 
    int beg, int end){
    if(beg > end){
        std::cerr<<"Empty command\n";
        updateErrorMessage("Parser: Empty command");
        return nullptr;
    }
    commandLineStruct newCommand;
    newCommand.cmdname = tokens[beg]->giveTokenValue();
    newCommand.args = {};
    newCommand.flags = {};
    for(int i = beg + 1; i <=end; i++){
        if(tokens[i] == nullptr){
            std::cerr<<"Parser: Error empty token during command parsing\n";
            updateErrorMessage("Parser: Error empty token during command parsing");
            return nullptr;
        }
        switch (tokens[i]->giveTokenType()) {
            case Tokens::FLAG:
                newCommand.flags.push_back(tokens[i]->giveTokenValue());
                break;
            case Tokens::ARG: {
                std::string argContent = tokens[i]->giveTokenValue();
                Ast newArgAst = std::make_shared<struct AstNode>(NodeType::Arg, argContent);
                newCommand.args.push_back(newArgAst);
                break;
            }
            default:
                std::cerr<<"Not implemented token:\n";
                tokens[i]->giveTokenValue();
                updateErrorMessage("Unknown token" + 
                    tokens[i]->giveTokenValue());
                break;
        }
    }
    Ast result = std::make_shared<AstNode>(NodeType::Command, newCommand);
    return result;
}

void debugCommand(Ast command){
    if(command ==nullptr){
        std::cerr<<"Null command\n";
        return;
    }
    if(command->type != NodeType::Command){
        std::cerr<<"This is not a command\n";
        return;
    }
    if (auto* cmd = std::get_if<commandLineStruct>(&command->value)) {
        std::cout << "<Command: " << cmd->cmdname;
        std::cout << "Flags: ";
        for(std::string flag : cmd->flags){
            std::cout << ", "<< flag;
        }
        std::cout << "Arguments: ";
        for(Ast arg : cmd->args){
            std::cout << ", ";
            debugArg(arg);
        }
        std::cout << ">\n";
    }
}

Ast Parser::parse(const std::vector<std::shared_ptr<Token>>& tokens, int beg, int end){
    int strongestIndex = this->findStrongestOperatorIndex(tokens, beg, end);
    std::shared_ptr<Token> strongestToken = tokens[strongestIndex];
    if(strongestToken == nullptr){
        std::cerr<<"Empty token during parsing\n";
        updateErrorMessage("Parser: Error empty token during command parsing");
        return nullptr;
    }
    switch (strongestToken->giveTokenType()){
        case Tokens::PIPE : {
            Ast leftParse = parse(tokens, beg, strongestIndex - 1);
            Ast rightParse = parse(tokens, strongestIndex + 1, end);
            pipeStruct newPipe;
            newPipe.left = leftParse;
            newPipe.right = rightParse;
            Ast newAst = std::make_shared<struct AstNode>(NodeType::Pipe, newPipe);
            return newAst;
        }
        case Tokens::ANDS : {
            Ast leftParse = parse(tokens, beg, strongestIndex - 1);
            Ast rightParse = parse(tokens, strongestIndex + 1, end);
            andStruct newPipe;
            newPipe.left = leftParse;
            newPipe.right = rightParse;
            Ast newAst = std::make_shared<struct AstNode>(NodeType::And, newPipe);
            return newAst;
        }
        case Tokens::CMDNAME : {
            return parseCommand(tokens, strongestIndex, end);
        }
        case Tokens::APPEND : {
            if(strongestIndex == end){
                std::cerr<<"Index out of range. Not filename given\n";
                updateErrorMessage("Not filename given");
                return nullptr;
            }
            if(tokens[strongestIndex + 1]->giveTokenType() != Tokens::ARG){
                std::cerr<<"Not arg token found to parse to find filename\n";
                updateErrorMessage("Wrong token (not an argument)"); // Poprawię, żeby wypisywało zły token
                return nullptr;
            }
            appendStruct app;
            app.filename = tokens[strongestIndex + 1]->giveTokenValue();
            app.toDo = { parse(tokens, beg, strongestIndex - 1) };
            return std::make_shared<AstNode>(NodeType::Append, app);
        }
        case Tokens::OWRITE : {
            Ast leftParse = parse(tokens, beg, strongestIndex - 1);
            if(strongestIndex == end){
                std::cerr<<"Index out of range. Not filename given\n";
                updateErrorMessage("Not filename given");
                return nullptr;
            }
            if(tokens[strongestIndex + 1]->giveTokenType() != Tokens::ARG){
                std::cerr<<"Not arg token found to parse to find filename\n";
                updateErrorMessage("Wrong token (not an argument)");;
                return nullptr;
            }
            overwriteStruct ovr;
            ovr.filename = tokens[strongestIndex + 1]->giveTokenValue();
            ovr.toDo = { parse(tokens, beg, strongestIndex - 1) };
            return std::make_shared<AstNode>(NodeType::Overwrite, ovr);
        }
        case Tokens::ARG:
            //std::cout<<"EEEEEEERRRRRRRRRRRRROOOOOOOOOOOORRRRRRR\n";
            std::cerr<<"Unbound argument " << strongestToken->giveTokenValue();
            updateErrorMessage("Unknown command " + strongestToken->giveTokenValue());
            return nullptr;
        case Tokens::FLAG:
            std::cerr<<"Unbound flag " << strongestToken->giveTokenValue();
            updateErrorMessage("Unbound flag " + strongestToken->giveTokenValue());
            return nullptr;
    }
    return nullptr;
}

void debugParsing(Ast expr){
    if(expr == nullptr){
        std::cerr<<"Debugging error: empty expression\n";
        return;
    }
    switch (expr-> type) {
        case NodeType::Command:
            debugCommand(expr);
            break;
        case NodeType::And:
            std::cout<<"(";
            if (auto* ands = std::get_if<andStruct>(&expr->value)){
                debugParsing(ands->left);
                std::cout<<"&&!!!AND!!!&&\n";
                debugParsing(ands->right);
            }
            std::cout<<")";
            break;
        case NodeType::Pipe:
            std::cout<<"(";
            if (auto* pipe = std::get_if<pipeStruct>(&expr->value)){
                debugParsing(pipe->left);
                std::cout<<"|!!!PIPE!!!|\n";
                debugParsing(pipe->right);
            }
            std::cout<<")";
            break;
        case NodeType::Append:
            if (auto* append = std::get_if<appendStruct>(&expr->value)){
                debugParsing(append->toDo);
                std::cout<<"<<!!!append!!!>>\n";
                std::cout<<append->filename<<"\n";
            }
            break;
        case NodeType::Overwrite:
            if (auto* owrite = std::get_if<overwriteStruct>(&expr->value)){
                debugParsing(owrite->toDo);
                std::cout<<"<!!!overWriting!!!>\n";
                std::cout<<owrite->filename<<"\n";
            }
            break;
        default:
            std::cerr<<"Ajejej!!!\n";
            break;
    }
}

Parser::Parser(){}

Ast Parser::initParsing(const std::vector<std::shared_ptr<Token>>& tokens){
    int end = tokens.size() - 1;
    if(tokens.empty()){
        updateErrorMessage("Empty token");
        return nullptr;
    }
    return parse(tokens, 0, end);
}
/*
int main(){
    std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>();
    std::string test;
    std::getline(std::cin, test);
    debugLexer(lexer->lexing(test));
    std::cout<<"\n";
    std::vector<std::shared_ptr<Token>> cmd = lexer->lexing(test);
    int beg = 0;
    int end = cmd.size() - 1;
    std::shared_ptr<Parser> prs = std::make_shared<Parser>();
    debugParsing(prs->parse(lexer->lexing(test), beg, end));
}*/