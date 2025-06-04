#include "User.hpp"
#include <string>

std::string User::giveUserName(){
    return this->username;
}

User::User(std::string username){
    this->username = username;
}