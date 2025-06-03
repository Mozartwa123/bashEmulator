#include "error.hpp"
#include <string>

std::string globalErrorMessage = "Error message:\n";

std::string clearErrorMessage(){
    std::string pom = globalErrorMessage;
    globalErrorMessage = "Error message:\n";
    return pom;
}

void updateErrorMessage(std::string newError){
    globalErrorMessage += newError;
    globalErrorMessage += "\n";
}