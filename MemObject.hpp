#pragma once
#include <string>
#include <memory>

class User;
class MemObject {
private:
  std::string path;
  std::string objname;
  //bool isScriptObject;
  std::shared_ptr<User> author;
  friend class MyDirectory;
  friend class File;
  friend class Computer;
  friend class Command;

  friend class Console;

public:
  std::string giveObjName();
  std::string giveAuthor();
  std::shared_ptr<User> giveAuthorObject();
};