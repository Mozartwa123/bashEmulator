#include <string>

enum class Priveledges{
    NOTHING,
    EVERYTHING //Później można dodać klasę GROUP i do niej dodawać użytkowników, ale to na razie starczy
};
class User{
    private:
        std::string username;
        Priveledges pvldg;
        std::string computerColor;
        std::string userColor;
        std::string monkeyColor;
        std::string promtChar;
    /*
        Prosta, ale jednak klasa... Dzięki jej obecności dodanie następnych funkcji, takie jak 
        przynależność do grup, logowanie się, itd. nie będzie tak bolało, niż gdybym zostawił
        użytkownika jako pojedynczy string.
    */
    public:
        User(std::string username);
        std::string giveUserName();
        void setPriveledges(Priveledges pvldg);
        void setPromntLook(std::string cmpColor, std::string usrColor, std::string mnkColor, std::string promntChar);
        std::string getCmpColor();
        std::string getUsrColor();
        std::string getMnkColor();
        std::string getPromtChar();
        Priveledges showPriveledges();
};