#ifndef INPUT_HEADER
#define INPUT_HEADER

#include <string_view>

inline int dashCount = 69;
inline std::string_view syntaxCLI = "./main <arguments> <Filenames>\n";

/*
    for grep 
    1. taking argument input
    2. parsing input
    3. file reading and parsing
    4. output based on parsing
*/

void checkArguments(int);
void printDashes();
void printSyntax();


#endif