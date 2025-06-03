#include <cstdio>
#include <string>
#include <vector>
#include "tokenizer.hpp"
#include <sstream>
#include <iostream>

using namespace std;

vector<string> lineSplit(string line){
    stringstream ss(line);
    string token;
    vector<string> result;
    while(ss >> token){
        result.push_back(token);
    }
    return result;
}

vector<string> generalSplit(string line, char separator){
    stringstream ss(line);
    string token;
    vector<string> result;
    while (getline(ss, token, separator)) {
        result.push_back(token);
    }
    return result;
}

vector<string> pathSplit(string line){
    vector<string> presult = generalSplit(line, '/');
    if(line[0] == '/'){
        presult[0] = "/";
    }
    if(line[0] == '/'){
        presult[0] = "/";
    }
    if(line[0]== '~'){
        presult[0] = "$(homeDirectory)";
    }
    return presult;
}

vector<string> scriptSplitFromStdin(){
    string token;
    vector<string> result;
    while (getline(cin, token)) {
        if(token == "end"){
            break;
        }
        result.push_back(token);
    }
    return result;
}

void debugSplit(vector<string> tokens){
    int i = 1;
    for(string token : tokens){
        fprintf(stdout, "token%i %s\n", i,token.c_str());
        i++;
    }
}