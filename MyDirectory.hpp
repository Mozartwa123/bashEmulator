#include "MemObject.hpp"
#include <vector>
#include <memory>
#include <string>

class File;
class MyDirectory : public MemObject, std::enable_shared_from_this<MyDirectory> {
public:
  std::shared_ptr<MyDirectory> parentDir;
  std::vector<std::shared_ptr<MyDirectory>> childrenDir;
  std::vector<std::shared_ptr<File>> childrenFil;

  MyDirectory(std::string name, std::shared_ptr<User> author,
              std::shared_ptr<MyDirectory> parent);

  static std::shared_ptr<MyDirectory> create(std::string name, std::shared_ptr<User> author,
                                        std::shared_ptr<MyDirectory> parent);

  void attachToParent();
  std::shared_ptr<MyDirectory> findChildDir(std::string name);
  std::shared_ptr<File> findChildFil(std::string name);
  std::string givePath();
};