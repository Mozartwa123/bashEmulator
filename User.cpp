#include "User.hpp"
#include <string>

std::string User::giveUserName() { return this->username; }

User::User(std::string username) {
  this->username = username;
  this->pvldg = Priveledges::NOTHING;
}

void User::setPriveledges(Priveledges pvldg) { this->pvldg = pvldg; }
/*
  Na bardzo wysokim statium projektu (który raczej nie nastąpi) można by było dodać odczytywanie
  .bashrc, a co z tym idzie wybranych przez użytkownika kolorów... Wtedy te pola mogą okazać się pomocne.
*/
void User::setPromntLook(std::string cmpColor, std::string usrColor,
                         std::string mnkColor, std::string promntChar) {
  this->computerColor = cmpColor;
  this->userColor = usrColor;
  this->monkeyColor = mnkColor;
  this->promtChar = promntChar;
}

std::string User::getUsrColor() { return this->userColor; }

std::string User::getCmpColor() { return this->computerColor; }

std::string User::getMnkColor() { return this->monkeyColor; }

std::string User::getPromtChar() { return this->promtChar; }

Priveledges User::showPriveledges() { return this->pvldg; }