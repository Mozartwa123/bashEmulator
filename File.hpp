#include <string>
#include <memory>
#include "MemObject.hpp"

class MemObject;
class MyDirectory;
class File : public MemObject{
public:
  std::shared_ptr<User> author;
  std::string filecontent;
  std::shared_ptr<MyDirectory> localization;
  File(std::string name, std::shared_ptr<User> author, std::shared_ptr<MyDirectory> localization);
  std::string givePath();
};