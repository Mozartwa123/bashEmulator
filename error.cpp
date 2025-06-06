#include "error.hpp"
#include <string>

std::string globalErrorMessage = "Error message:\n";
bool errorOccured = false;

std::string clearErrorMessage(){
    std::string pom = globalErrorMessage;
    globalErrorMessage = "Error message:\n";
    errorOccured = false;
    return pom;
}

void updateErrorMessage(std::string newError){
    globalErrorMessage += newError;
    errorOccured = true;
    globalErrorMessage += "\n";
}