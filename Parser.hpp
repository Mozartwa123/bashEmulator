//Parser.hpp
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include "Lexer.hpp"

struct AstNode;
using Ast = std::shared_ptr<AstNode>;

enum class NodeType {
    Command,
    Pipe,
    Overwrite,
    Append,
    And,
    Arg
};

struct commandLineStruct {
    std::string cmdname;
    std::vector<std::string> flags;
    std::vector<std::string> args;
};

struct pipeStruct {
    Ast left;
    Ast right;
};

struct appendStruct {
    std::string filename;
    Ast toDo;
};

struct overwriteStruct {
    std::string filename;
    Ast toDo;
};

struct andStruct {
    Ast left;
    Ast right;
};

struct AstNode {
    NodeType type;
    std::variant<
        commandLineStruct,
        pipeStruct,
        overwriteStruct,
        appendStruct,
        andStruct,
        std::string // Arg
    > value;

    template <typename T>
    AstNode(NodeType t, T val) : type(t), value(std::move(val)) {}
};
void debugParsing(Ast expr);
bool bindingCompare(std::shared_ptr<Token> tkn1, std::shared_ptr<Token> tkn2);
class Parser{
    public:
        Parser();
        int findStrongestOperatorIndex(const std::vector<std::shared_ptr<Token>>& tokens, int beg, int end);
        Ast initParsing(const std::vector<std::shared_ptr<Token>>& tokens);
        Ast parse(const std::vector<std::shared_ptr<Token>>& tokens, int beg, int end);
        Ast parseCommand(const std::vector<std::shared_ptr<Token>>& tokens, int beg, int end);
};