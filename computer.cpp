#include "Parser.hpp"
#include "Lexer.hpp"
#include "User.hpp"
#include "tokenizer.hpp"
#include "error.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include "MemObject.hpp"
#include "File.hpp"
#include "MyDirectory.hpp"

using namespace std;

/*
    Niestety za późno zorientowałem się, jak TRUDNE jest rozdzielanie plików w
   c++ (nagłówki bez namespace std; implementacje metod w pliku cpp). Zmiana
   idei kosztowała by mnie zaczynanie od początku (nawarstwienie problemów). Z
   tego powodu (prawie) wszystko jest w jednym pliku. Przepraszam zatrudności.
*/
class File;
class MemObject;
class MyDirectory;
class Computer;
class Command;
// class Lexer;
// class Parser;
class Console;
/*
class MemObject {
private:
  string path;
  string objname;
  //bool isScriptObject;
  shared_ptr<User> author;
  friend class MyDirectory;
  friend class File;
  friend class Computer;
  friend class Command;

  friend class Console;

public:
  string giveObjName() { return this->objname; }
  string giveAuthor() { return this->author->giveUserName(); }
  shared_ptr<User> giveAuthorObject() { return this->author; }
};

class MyDirectory : public MemObject, enable_shared_from_this<MyDirectory> {
public:
  shared_ptr<MyDirectory> parentDir;
  vector<shared_ptr<MyDirectory>> childrenDir;
  vector<shared_ptr<File>> childrenFil;

  MyDirectory(string name, shared_ptr<User> author,
              shared_ptr<MyDirectory> parent) {
    this->objname = name;
    this->author = author;
    this->childrenDir = vector<shared_ptr<MyDirectory>>();
    this->childrenFil = vector<shared_ptr<File>>();
    this->parentDir = parent;
    this->path = "";
    // cout<<"creating"<<name;
    
    if(parentDir!=nullptr){
        (parentDir->childrenDir).push_back(shared_from_this());
    }*/
/*
  }

  static shared_ptr<MyDirectory> create(string name, shared_ptr<User> author,
                                        shared_ptr<MyDirectory> parent) {
    shared_ptr<MyDirectory> dir(new MyDirectory(name, author, parent));
    if (parent) {
      parent->childrenDir.push_back(dir);
    }
    return dir;
  }

  void attachToParent() {
    if (this->parentDir != nullptr) {
      this->parentDir->childrenDir.push_back(shared_from_this());
    }
  }
  shared_ptr<MyDirectory> findChildDir(string name) {
    for (shared_ptr<MyDirectory> dir : childrenDir) {
      if (dir->objname == name) {
        return dir;
      }
    }
    return nullptr;
  }
*/
/*
  shared_ptr<MyDirectory> findChildFil(string name) {
    for (shared_ptr<File> file : childrenFil) {
      if (file->giveObjName() == name) {
        return file;
      }
    }
    return nullptr;
  }

  string givePath() {
    if (this->path == "") {
      this->path =
          (this->giveObjName() == "/" ? "/"
                                      : (this->parentDir->giveObjName() == "/"
                                             ? "/" + this->giveObjName()
                                             : this->parentDir->givePath() +
                                                   "/" + this->giveObjName()));
    }
    return this->path;
  }
};

class File : public MemObject{
public:
  shared_ptr<User> author;
  string filecontent;
  shared_ptr<MyDirectory> localization;

  File(string name, shared_ptr<User> author, shared_ptr<MyDirectory> localization) {
    this->path = "";
    this->objname = name;
    this->filecontent = "";
    this->localization = localization;
    this->author = author;
  }
  string givePath() {
    if (this->path == "") {
      this->path = (this->giveObjName() == "/"
                        ? "/"
                        : (this->localization->giveObjName() == "/"
                               ? "/" + this->giveObjName()
                               : this->localization->givePath() + "/" +
                                     this->giveObjName()));
    }
    return this->path;
  }
};
*/
class Computer {
public:
  vector<shared_ptr<User>> users;
  shared_ptr<User> currentUser;
  string cmpname;
  shared_ptr<MyDirectory> userDirectory;
  shared_ptr<MyDirectory> rootDirectory;
  friend class Command;
  friend class Console;

  string giveCurrentUserName(){
    return this->currentUser->giveUserName();
  }
  Computer(string username, string cmpname) {
    shared_ptr<User> rootUser = make_shared<User>("root");
    this->users = {};
    users.push_back(rootUser);
    shared_ptr<User> currentUser = make_shared<User>(username);
    this->currentUser = currentUser;
    users.push_back(currentUser);
    shared_ptr<MyDirectory> rootDirectory =
        MyDirectory::create("/", rootUser, nullptr);
    this->rootDirectory = rootDirectory;
    //(rootDirectory);
    MyDirectory::create("etc", rootUser, rootDirectory);
    shared_ptr<MyDirectory> homeDirectory =
        MyDirectory::create("home", rootUser, rootDirectory);
    // this -> homeDirectory = homeDirectory;
    //(homeDirectory);
    // (MyDirectory::create("home", false,
    // rootDirectory));
    MyDirectory::create("proc", rootUser, rootDirectory);
    (MyDirectory::create("lib", rootUser, rootDirectory));
    (MyDirectory::create("root", rootUser, rootDirectory));
    (MyDirectory::create("dev", rootUser, rootDirectory));
    (MyDirectory::create("bin", rootUser, rootDirectory));
    (MyDirectory::create("boot", rootUser, rootDirectory));
    (
        MyDirectory::create("lib64", rootUser, rootDirectory));
    (MyDirectory::create("mnt", rootUser, rootDirectory));
    (MyDirectory::create("opt", rootUser, rootDirectory));
    (MyDirectory::create("run", rootUser, rootDirectory));
    (MyDirectory::create("usr", rootUser, rootDirectory));
    (MyDirectory::create("var", rootUser, rootDirectory));
    this->cmpname = cmpname;
    /* TODO: USER DIRECTORY!!!!*/
    this->currentDirectory =
        MyDirectory::create(username, currentUser, homeDirectory);
    this->userDirectory = this->currentDirectory;
    (
        MyDirectory::create("documents", currentUser, this->userDirectory));
    (
        MyDirectory::create(".config",currentUser, this->userDirectory));
    (
        MyDirectory::create(".local", currentUser, this->userDirectory));
  }
  //static vector<shared_ptr<MyDirectory>> allDirectories;
  //static vector<shared_ptr<MyDirectory>> allFiles;
  static vector<shared_ptr<MyDirectory>> allCommands;
  shared_ptr<MyDirectory> currentDirectory;
  /*
  shared_ptr<MyDirectory> find_dir(string name) {
    for (size_t i = 0; i < allDirectories.size(); i++) {
      if (allDirectories[i]->objname == name) {
        return allDirectories[i];
      }
    }
    return nullptr;
  }
  shared_ptr<MyDirectory> find_fil(string name) {
    for (size_t i = 0; i < allFiles.size(); i++) {
      if (allFiles[i]->objname == name) {
        return allFiles[i];
      }
    }
    return nullptr;
  }*/
};

enum class SafetyStatus {
  AbsolutelySafe,
  SafeInScriptsOnly,
  RequiresRoot,
  DeadlyUnsafe
};

class Command {
public:
  string commandName;
  string description;
  map<string, SafetyStatus> availableFlags;
  function<string(vector<string> flags, vector<string> arguments,
                  shared_ptr<Computer>, shared_ptr<Command>)>
      execute;

  friend class Console;
  Command(string name, string desc, map<string, SafetyStatus> flags,
          function<string(vector<string>, vector<string>, shared_ptr<Computer>,
                          shared_ptr<Command>)>
              func)
      : commandName(name), description(desc), availableFlags(flags),
        execute(func) {}
};

string cd(vector<string> flags, vector<string> arguments,
          shared_ptr<Computer> computer, shared_ptr<Command> command) {
  shared_ptr<MyDirectory> dir = nullptr;
  /* flags logic */
  if (arguments.size() > 1 && command != nullptr) {
    fprintf(stderr, "To many arguments %s \n", command->description.c_str());
    // range_error("To many arguments " + command->description + "\n");
  }
  if (arguments.size() == 0) {
    shared_ptr<MyDirectory> dir = computer->userDirectory;
    computer->currentDirectory = dir;
    return dir->giveObjName();
  }
  int argsize = arguments[0].size();
  if (arguments[0] == ".") {
    fprintf(stdout, "entering directory %s \n",
            computer->currentDirectory->giveObjName().c_str());//zmienię to na logi, ale nie teraz, błagam!
    return computer->currentDirectory
        ->giveObjName(); // przechodzimy do tego samego katalogu
  } else if (arguments[0] == "..") {
    if (computer->currentDirectory->parentDir == nullptr) {
      fprintf(stderr, "/ directory does not have parent\n");
      return "";
    }
    computer->currentDirectory =
        computer->currentDirectory->parentDir; // przechodzimy do rodzica
    return computer->currentDirectory->giveObjName();
  } else if (arguments[0][0] == '/') {
    vector path = pathSplit(arguments[0]);
    debugSplit(path);
    computer->currentDirectory = computer->rootDirectory;
    int pathsize = path.size();
    for (int i = 1; i < pathsize; i++) {
      cd({}, {path[i]}, computer, command);
    }
    return computer->currentDirectory->giveObjName();
  } else if (arguments[0][0] == '~') {
    vector path = pathSplit(arguments[0]);
    debugSplit(path);
    computer->currentDirectory = computer->userDirectory;
    int pathsize = path.size();
    for (int i = 1; i < pathsize; i++) {
      cd({}, {path[i]}, computer, command);
    }
    return computer->currentDirectory->giveObjName();
  } else if (arguments[0].find("/") != string::npos) {
    vector path = pathSplit(arguments[0]);
    debugSplit(path);
    // computer -> currentDirectory = computer -> userDirectory;
    int pathsize = path.size();
    for (int i = 0; i < pathsize; i++) {
      cd({}, {path[i]}, computer, command);
    }
    return computer->currentDirectory->giveObjName();
  } else {
    dir = computer->currentDirectory->findChildDir(arguments[0]);
    if (dir == nullptr) {
      fprintf(stderr, "Given directory %s does not exist\n",
              (computer->currentDirectory->giveObjName() +
               (computer->currentDirectory->giveObjName() == "/" ? "" : "/") +
               arguments[0])
                  .c_str());
      return "";
    }
  }
  if (dir == nullptr) {
    fprintf(stderr, "not considering case %s\n", arguments[0].c_str());
    return "";
  }
  fprintf(stdout, "Entering directory %s\n", dir->giveObjName().c_str());
  computer->currentDirectory = dir;
  return dir->giveObjName();
}

string mkdirLoop(vector<string> flags, vector<string> arguments,
                  shared_ptr<Computer> computer, int idx, int argsize) {
  if (idx >= argsize) {
    return "";
  }
  vector<string> cutPath = pathSplitLast(arguments[idx]);
  shared_ptr<MyDirectory> parDir = computer->currentDirectory;
  string dirName = arguments[idx];
  if (!cutPath.empty()) {
    string path = cutPath[0];
    dirName = cutPath[1];
    shared_ptr<MyDirectory> curDir = computer->currentDirectory;
    cd({}, {path}, computer, nullptr);
    parDir = computer->currentDirectory;
    computer->currentDirectory = curDir;
  }
  //cout<<"created directory" + dirName + "\n";
  shared_ptr<MyDirectory> isAlreadyCreated = parDir->findChildDir(dirName);
  if(isAlreadyCreated!=nullptr){
    updateErrorMessage("Directory " + isAlreadyCreated -> givePath() + " already exists");
    return mkdirLoop(flags, arguments, computer, ++idx, argsize);
  }
  MyDirectory::create(dirName, computer->currentUser, parDir);

  return mkdirLoop(flags, arguments, computer, ++idx, argsize);
}

string mkdir(vector<string> flags, vector<string> arguments,
          shared_ptr<Computer> computer, shared_ptr<Command> command){
            if(arguments.empty()){
              updateErrorMessage("No directory name given. Command usage:\n"+command->description);
            } else mkdirLoop(flags, arguments, computer, 0, arguments.size());
            return "";
          }

string touchLoop(vector<string> flags, vector<string> arguments,
                  shared_ptr<Computer> computer, int idx, int argsize) {
  if (idx >= argsize) {
    return "";
  }
  vector<string> cutPath = pathSplitLast(arguments[idx]);
  shared_ptr<MyDirectory> parDir = computer->currentDirectory;
  string fileName = arguments[idx];
  if (!cutPath.empty()) {
    string path = cutPath[0];
    fileName = cutPath[1];
    shared_ptr<MyDirectory> curDir = computer->currentDirectory;
    cd({}, {path}, computer, nullptr);
    parDir = computer->currentDirectory;
    computer->currentDirectory = curDir;
  }
  //cout<<"created directory" + dirName + "\n";
  shared_ptr<File> isAlreadyCreated = parDir->findChildFil(fileName);
  if(isAlreadyCreated!=nullptr){
    updateErrorMessage("File " + isAlreadyCreated -> givePath() + " already exists");
    return touchLoop(flags, arguments, computer, ++idx, argsize);
  }
  shared_ptr<File> newFile = make_shared<File>(fileName, parDir->giveAuthorObject(), parDir);
  //cout<<"PLIIIK!!!";
  parDir->childrenFil.push_back(newFile);
 //cout<<"PLIIIK!!!";
  return touchLoop(flags, arguments, computer, ++idx, argsize);
}

string touch(vector<string> flags, vector<string> arguments,
             shared_ptr<Computer> computer, shared_ptr<Command> command) {
  if (arguments.empty()) {
    updateErrorMessage("No file name given. Command usage:\n" +
                       command->description);
  } else
    touchLoop(flags, arguments, computer, 0, arguments.size());
  return "";
}

string clear(vector<string> flags, vector<string> arguments,
             shared_ptr<Computer> computer, shared_ptr<Command> command) {
  //cout << "\033[2J\033[1;1H";
  return "\033[2J\033[1;1H";
}

string fastfetch(vector<string> flags, vector<string> arguments,
                 shared_ptr<Computer> computer, shared_ptr<Command> command) {
  ifstream file("ascii_logo.txt");
  ostringstream output;

  if (!file) {
    output << "ascii logo file not found\n";
    return output.str();
  }

  vector<string> logoLines;
  string line;
  while (getline(file, line)) {
    logoLines.push_back(line);
  }

  string userHeader = computer->giveCurrentUserName() + "@" + computer->cmpname;
  string dashes = "";
  int userHeaderSize = userHeader.size();
  for (int i = 0; i < userHeaderSize; i++) {
    dashes += "-";
  }

  vector<string> infoLines = {
      userHeader,
      dashes,
      "Host: Altair 8800",
      "OS: noKernelOS vide steamUS III",
      "Shell: bashEmulator",
      "Memory: 42 GiB",
      "CPU: e8400",
      "GPU: GeForce RTX 5060",
      "Disk: 105GiB/250GiB [42%]",
      "Kernel: none",
      "Packages: 1 [snake]", // skoro jest pacman, to dlaczego nie ma być
                             // snake'a?
      "Terminal: tty42",
      "Battery: 42% (not charging)",
      "Locale: en_US.UTF-8"};

  // Wypisz razem
  size_t maxLines = max(logoLines.size(), infoLines.size());
  for (size_t i = 0; i < maxLines; ++i) {
    string logo = (i < logoLines.size()) ? logoLines[i] : "";
    string info = (i < infoLines.size()) ? infoLines[i] : "";

    output << CYAN << left << setw(30) << logo << "  " << BLUE << info << RESET
           << '\n';
  }

  //cout << output.str();
  return output.str();
}

string pwd(vector<string> flags, vector<string> arguments,
           shared_ptr<Computer> computer, shared_ptr<Command> command) {
  if (arguments.size() != 0 && flags.size() != 0) {
    fprintf(stderr, "To many arguments %s \n", command->description.c_str());
    // range_error("To many arguments " + command->description + "\n");
  }
  //fprintf(stdout, "Current directory %s \n",
          //computer->currentDirectory->givePath().c_str());
  return computer->currentDirectory->givePath().c_str();
}

string listRec(shared_ptr<MyDirectory> listedDir, vector<string> flags,
               ostringstream &output) {
  output << RESET;
  if (!listedDir->childrenDir.empty()) {
    output << endl;
  }
  for (shared_ptr<File> fil : listedDir->childrenFil) {
    //cout<<"WYpisuje, ale nie wypisuje\n";
    if (fil->giveObjName()[0] == '.' &&
        find(flags.begin(), flags.end(), "a") == flags.end()) {
      continue;
    }
    //cout<< fil->giveObjName();
    output << fil->giveObjName() << "\t";
  }
  output << BLUE;
  for (shared_ptr<MyDirectory> dir : listedDir->childrenDir) {
    if (dir->giveObjName()[0] == '.' &&
        find(flags.begin(), flags.end(), "a") == flags.end()) {
      continue;
    }
    if (find(flags.begin(), flags.end(), "R") != flags.end()) {
      output << dir->givePath() << endl;
      listRec(dir, flags, output);
      // output<<"\n";
    } else {
      output << dir->giveObjName() << "\t";
    }
  }
  // output << RESET;
  output << "\n";
  output << RESET;
  string result = output.str();
  // cout << result;
  // putchar('\n');
  return result;
}

string ls(vector<string> flags, vector<string> arguments,
          shared_ptr<Computer> computer, shared_ptr<Command> command) {
  shared_ptr<MyDirectory> listedDir;
  if (arguments.size() == 0) {
    listedDir = computer->currentDirectory;
  } else {
    shared_ptr<MyDirectory> pom = computer->currentDirectory;
    cd(flags, arguments, computer, nullptr);
    listedDir = computer->currentDirectory;
    computer->currentDirectory = pom;
  }
  ostringstream output;
  string result = listRec(listedDir, flags, output);
  result += "\n";
  //cout << result;
  cout << RESET;
  return result;
}

class Interpreter {
public:
  shared_ptr<Parser> parser;
  shared_ptr<Lexer> lexer;
  shared_ptr<Computer> computer;
  static map<string, shared_ptr<Command>> availableCommands;
  Interpreter(shared_ptr<Computer> computer) {
    availableCommands["cd"] = make_shared<Command>(
        "cd", "entering directories", map<string, SafetyStatus>{}, cd);
    availableCommands["pwd"] =
        make_shared<Command>("pwd", "showing current localization",
                             map<string, SafetyStatus>{}, pwd);
    availableCommands["fastfetch"] =
        make_shared<Command>("fastfetch", "showing technical info",
                             map<string, SafetyStatus>{}, fastfetch);
    availableCommands["clear"] = make_shared<Command>(
        "clear", "clearing console", map<string, SafetyStatus>{}, clear);
    availableCommands["ls"] = make_shared<Command>(
        "ls", "listing directory content", map<string, SafetyStatus>{}, ls);
    availableCommands["mkdir"] = make_shared<Command>(
        "mkdir", "creating directories. Usage mkdir [dir1] [dir2] ...", map<string, SafetyStatus>{}, mkdir);
    availableCommands["touch"] = make_shared<Command>(
        "touch", "creating files. Usage touch [file1] [file2] ...", map<string, SafetyStatus>{}, touch);
    this->computer = computer;
    this->lexer = make_shared<Lexer>();
    this->parser = make_shared<Parser>();
  }
  string evalArg(Ast potArg);
  Ast syntaxConverting(string input) {
    vector<shared_ptr<Token>> lexed = lexer->lexing(input);
    //debugLexer(lexed);
    Ast parsed = parser->initParsing(lexed);
    //debugParsing(parser->initParsing(lexer->lexing(input)));
    return parsed;
  }
  string evalCmd(string cmdname, vector<Ast> &args, vector<string> &flags) {
    shared_ptr<Command> cmd = nullptr;
    auto it = availableCommands.find(cmdname);
    if (it == availableCommands.end()) {
      fprintf(stderr, "unknown command %s\n", cmdname.c_str());
      return "";
    }
    cmd = it->second;
    if (flags.size() > 0 && flags[0] == "help") {
      printf("%s\n", cmd->description.c_str());
      return cmd->description.c_str();
    }
    vector<string> evaluatedArgs = {};
    for(Ast potArg : args){
        if(potArg == nullptr){
            cerr<<"Empty argument\n";
            continue; //kontrowersyjne, nie wiem, czemu to robię, ale to nietrudno zmienić ewentualnie...
        }
        evaluatedArgs.push_back(evalArg(potArg));
    }
    return cmd->execute(flags, evaluatedArgs, this->computer, cmd);
  }
  string evalFromAst(Ast cmdLine, vector<string> pipeStack) {
    if(cmdLine == nullptr){
        cerr<<"Nullptr during evaluation\n";
        updateErrorMessage("Parser failed");
        return clearErrorMessage();
    }
    switch (cmdLine->type) {
    case NodeType::Command:
      if (auto *cmd = std::get_if<commandLineStruct>(&cmdLine->value)) {
        for (string e : pipeStack) {
          Ast newArgAst = std::make_shared<struct AstNode>(NodeType::Arg, e);
          cmd->args.push_back(newArgAst);
        }
        return evalCmd(cmd->cmdname, cmd->args, cmd->flags);
      }

    case NodeType::And:
      if (auto *ands = std::get_if<andStruct>(&cmdLine->value)) {
        return evalFromAst(ands->left, pipeStack) +
               evalFromAst(ands->right, pipeStack);
      }
    case NodeType::Pipe: {
      if (auto *pipe = std::get_if<pipeStruct>(&cmdLine->value)) {
        string pipeLeft = evalFromAst(pipe->left, pipeStack);
        pipeStack.push_back(pipeLeft);
        return evalFromAst(pipe->right, pipeStack);
      }
    }
    default:
      cerr << "Yet not implemented...\n";
      return clearErrorMessage();
    }
  }
  string eval(string input) { return evalFromAst(syntaxConverting(input), {}); }
};

string Interpreter::evalArg(Ast potArg) {
  if (potArg == nullptr){
    cerr<<"Empty argument\n";
    return "Empty argument";
  }
  if (potArg->type != NodeType::Arg) {
    return this->evalFromAst(potArg, {}); // rekurencja ogonowa
  }
  if (auto *arg = std::get_if<string>(&potArg->value)) {
    return *arg;
  }
    updateErrorMessage("Interpreter error");
    return clearErrorMessage();
}

class Console {
public:
  shared_ptr<Computer> computer;
  shared_ptr<Interpreter> interpreter;
  Console() {
    printf("WELCOME TO BASH EMULATOR\n");
    // do skanowania zrobię chyba flagę uruchomienia, ale jeśli skuszę się na
    // GUI to będzie problem...
    string cmpName;
    string usrName;
    printf("How would you like to name your linux machine?\n");
    cin >> cmpName;
    printf("How would you like to name yourself?\n");
    cin >> usrName;
    this->computer = make_shared<Computer>(usrName, cmpName);
    this->interpreter = make_shared<Interpreter>(this->computer);
  }
  void bashSessionInit() {
    string n;
    getline(cin, n);
    bashSession();
  }

private:
  void bashSession() {
    cout << RED << this->computer->giveCurrentUserName() << MAGENTA << "@" << GREEN
         << computer->cmpname + " " << YELLOW
         << ((computer->currentDirectory == computer->userDirectory)
                 ? "~"
                 : computer->currentDirectory->giveObjName())
         << RESET << " $ ";
    string n;
    getline(cin, n);
    if (n == "end") {
      return;
    }
    // if(n == "") bashSession();
    string message = interpreter->eval(n);
    cout<<message;
    cout << RESET;
    bashSession();
  }
};

map<string, shared_ptr<Command>> Interpreter::availableCommands = {};
//vector<shared_ptr<MyDirectory>> Computer::allDirectories = {};
//vector<shared_ptr<MyDirectory>> Computer::allFiles = {};

int main(void) {

  shared_ptr<Console> newSession = make_shared<Console>();
  newSession->bashSessionInit();

  /*
  shared_ptr<Computer> cmp = make_shared<Computer>("a", "b");
  shared_ptr<Interpreter> intrp = make_shared<Interpreter>(cmp);
  intrp->eval("cd ..");
  */
  /*
  shared_ptr<Parser> prs = make_shared<Parser>();
  shared_ptr<Lexer> lx = make_shared<Lexer>();
  string input = "cd -fff  --fgh ..";
  debugParsing(prs->initParsing(lx->lexing(input)));
  */
}