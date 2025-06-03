#include <string>

#ifndef ERROR_H
#define ERROR_H

extern std::string globalErrorMessage;

#endif

std::string clearErrorMessage();
void updateErrorMessage(std::string newError);