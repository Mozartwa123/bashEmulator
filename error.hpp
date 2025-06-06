#include <string>

#ifndef ERROR_H
#define ERROR_H

extern std::string globalErrorMessage;
extern bool errorOccured;

#endif

std::string clearErrorMessage();
void updateErrorMessage(std::string newError);