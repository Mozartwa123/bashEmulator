#include "File.hpp"
#include "MyDirectory.hpp"
#include "User.hpp"
#include <memory>
#include <string>

std::string MemObject::giveObjName() { return this->objname; }
std::string MemObject::giveAuthor() { return this->author->giveUserName(); }
std::shared_ptr<User> MemObject::giveAuthorObject() { return this->author; }

MyDirectory::MyDirectory(std::string name, std::shared_ptr<User> author,
                         std::shared_ptr<MyDirectory> parent) {
  this->objname = name;
  this->author = author;
  this->childrenDir = std::vector<std::shared_ptr<MyDirectory>>();
  this->childrenFil = std::vector<std::shared_ptr<File>>();
  this->parentDir = parent;
  this->path = "";
}

std::shared_ptr<MyDirectory>
MyDirectory::create(std::string name, std::shared_ptr<User> author,
                    std::shared_ptr<MyDirectory> parent) {
  std::shared_ptr<MyDirectory> dir(new MyDirectory(name, author, parent));
  if (parent) {
    parent->childrenDir.push_back(dir);
  }
  return dir;
}

void MyDirectory::attachToParent() {
  if (this->parentDir != nullptr) {
    this->parentDir->childrenDir.push_back(shared_from_this());
  }
}

std::shared_ptr<MyDirectory> MyDirectory::findChildDir(std::string name) {
  for (std::shared_ptr<MyDirectory> dir : this->childrenDir) {
    if (dir->objname == name) {
      return dir;
    }
  }
  return nullptr;
}

std::shared_ptr<File> MyDirectory::findChildFil(std::string name) {
  for (std::shared_ptr<File> file : childrenFil) {
    if (file->giveObjName() == name) {
      return file;
    }
  }
  return nullptr;
}

std::string MyDirectory::givePath() {
  if (this->path == "") {
    this->path =
        (this->giveObjName() == "/"
             ? "/"
             : (this->parentDir->giveObjName() == "/"
                    ? "/" + this->giveObjName()
                    : this->parentDir->givePath() + "/" + this->giveObjName()));
  }
  return this->path;
}

File::File(std::string name, std::shared_ptr<User> author,
           std::shared_ptr<MyDirectory> localization) {
  this->path = "";
  this->objname = name;
  this->filecontent = "#!/bin/bash";
  this->localization = localization;
  this->author = author;
  //if(this->author!=nullptr){
    //std::cout<<"author of this file will be"<<this->author->giveUserName();
  //}
}
std::string File::givePath() {
  if (this->path == "") {
    this->path =
        (this->giveObjName() == "/" ? "/"
        : (this->localization->giveObjName() == "/"
        ? "/" + this->giveObjName()
        : this->localization->givePath() +
        "/" + this->giveObjName()));
  }
  return this->path;
}

std::string File::giveContent(){
  return this->filecontent;
}
void File::setContent(std::string newContent){
  this->filecontent = newContent;
}

void File::appendContent(std::string appendedContent){
  this->filecontent += appendedContent;
}