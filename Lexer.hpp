//Lexer.hpp
#include <vector>
#include <string>
#include <string>
#include <optional>
#include <string>
#include <optional>
#include <memory>

enum class Tokens{
    CMDNAME,
    ARG,
    FLAG,
    PIPE,
    ANDS,
    APPEND,
    OWRITE,
    /* i wiele więcej */
};

enum class Binding{
    LEFT,
    OTHER
};
class Token{
    private:
        Tokens tokenType;
        std::optional<std::string> value;
        Binding bindRule;
        int priority;
    public:
        Token(Tokens tokenType, std::optional<std::string> value, int priority, Binding bindRule);
        Binding giveBindingType();
        Tokens giveTokenType();
        std::string giveTokenValue();
        int givePriority();
        void debugToken();
};

void debugLexer(std::vector<std::shared_ptr<Token>> tokenList);
class Lexer{
    public:
        Lexer();
        std::vector<std::shared_ptr<Token>> lexing(std::string& input);
};