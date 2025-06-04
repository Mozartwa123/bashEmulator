#include <string>

class User{
    private:
        std::string username;
    /*
        Prosta, ale jednak klasa... Dzięki jej obecności dodanie następnych funkcji, takie jak 
        przynależność do grup, logowanie się, itd. nie będzie tak bolało, niż gdybym zostawił
        użytkownika jako pojedynczy string.
    */
    public:
        User(std::string username);
        std::string giveUserName();
};