#ifndef INPUT_HEADER
#define INPUT_HEADER

#include <string_view>



struct GrepSettings
{
    bool caseInsesitive = false;
    bool recursive = false;


    std::string_view pattern = "";
    std::vector<std::string> fileNames;
};

GrepSettings parseInput(int argc, const char* argv[]);

namespace Input
{
    inline bool lessArgs = false;
    inline short int minArgs = 3;
    inline short int dashCount = 50;
    inline std::string_view syntaxCLI = "./main <arguments> <Finding Keyword> <Filenames>\n";
}

void checkArguments(int);
void printDashes();
void printSyntax();


#endif