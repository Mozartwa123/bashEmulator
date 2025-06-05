//Lexer.cpp
#include "Lexer.hpp"
#include "error.hpp"
#include "tokenizer.hpp"
#include <optional>
#include <ostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>

/*
    Implementacja metody lexing z Lexer.hpp
*/
std::vector<std::string> implementedCommands{
    "cd",
    "touch",
    "pwd",
    "fastfetch",
    "ls",
    "clear",
    "mkdir",
};

Token::Token(Tokens tokenType, std::optional<std::string> value, int priority, Binding bindRule)
    : tokenType(tokenType), value(std::move(value)), priority(priority), bindRule(bindRule) {}

Binding Token::giveBindingType(){
    return this->bindRule;
}
Tokens Token::giveTokenType(){
    return this->tokenType;
}
std::string Token::giveTokenValue(){
    if(this->value.has_value()){
        return this->value.value();
    }
    return "";
}

int Token::givePriority(){
    return this->priority;
}

void Token::debugToken(){
    switch (this->tokenType){
        case Tokens::CMDNAME:
            //std::cout<<"CMD ";
            break;
        case Tokens::APPEND:
            //std::cout<<"APPEND ";
            break;
        case Tokens::OWRITE:
            //std::cout<<"OWRITE ";
            break;
        case Tokens::PIPE:
            //std::cout<<"PIPE ";
            break;
        case Tokens::ANDS:
            //std::cout<<"&& ";
            break;
        case Tokens::FLAG:
            //std::cout<<"Flag";
            break;
        case Tokens::ARG:
            //std::cout<<"Arg";
            break;
    }
    std::cout<<this->giveTokenValue()<<" ";
    std::cout<<this->givePriority()<<"\n";
};

void debugLexer(std::vector<std::shared_ptr<Token>> tokenList){
    for(std::shared_ptr<Token> tkn : tokenList){
        tkn->debugToken();
    }
};

Lexer::Lexer(){}

std::vector<std::shared_ptr<Token>> Lexer::lexing(std::string& input){
    std::vector<std::shared_ptr<Token>> tokenList = {};
    std::vector<std::string> words = lineSplit(input);
    for(std::string word : words){
        std::cout<<word;
        if(word == ""){}
        else if(word=="|"){
            std::cout<<"is Pipe\n";
            tokenList.push_back(std::make_shared<Token>(Tokens::PIPE, std::nullopt,4, Binding::LEFT));
        } else if (word==">>"){
            std::cout<<"is Append\n";
            tokenList.push_back(std::make_shared<Token>(Tokens::APPEND, std::nullopt, 7, Binding::LEFT));
        } else if (word==">"){
            std::cout<<"is Overwrite\n";
            tokenList.push_back(std::make_shared<Token>(Tokens::OWRITE, std::nullopt, 7, Binding::LEFT));
        } else if(word=="&&"){
            std::cout<<"is And\n";
            tokenList.push_back(std::make_shared<Token>(Tokens::ANDS, std::nullopt, 3, Binding::LEFT));
        } else if(std::find(implementedCommands.begin(), implementedCommands.end(), word) != implementedCommands.end()){
            std::cout<<word;
            std::cout << "DEBUG: implementedCommands:\n";
for (const auto& cmd : implementedCommands) {
    std::cout << "[" << cmd << "] ";
}
            std::cout<<"is command\n";
            tokenList.push_back(std::make_shared<Token>(Tokens::CMDNAME, word, 1, Binding::LEFT));
        } else if (word.size() > 2 && word[0] == '-' && word[1] == '-'){
            std::cout<<"is chainflag\n";
            tokenList.push_back(std::make_shared<Token>(Tokens::FLAG, word.substr(2), 0, Binding::LEFT));
        } else if (word.size() > 1 && word[0] == '-'){
            std::cout<<"is charflag\n";
            word = word.substr(1);
            for(char c : word){
                tokenList.push_back(std::make_shared<Token>(Tokens::FLAG, std::string(1, c), 0, Binding::LEFT));//chcę zrzutować c na stringa
            }
        } else {
            std::cout<<"is argument\n";
            tokenList.push_back(std::make_shared<Token>(Tokens::ARG, word, 0, Binding::OTHER));
        }
    }
    return tokenList;
}

/*
int main(){
    std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>();
    std::string test;
    std::getline(std::cin, test);
    debugLexer(lexer->lexing(test));
}*/