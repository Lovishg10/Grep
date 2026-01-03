#include <iostream>
#include <stdexcept>
#include <string_view>
#include <handleInput.h>

/*
    for grep 
    1. taking argument input
    2. parsing input
    3. file reading and parsing
    4. output based on parsing
*/
void parseInput(int argc, const char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        
    }
}

int main (int argc, const char*  argv[])
{
    
    try
    { // throws std::runtime exception if less than 3 arguments
        checkArguments(argc);

        parseInput(argc, argv);

    }
    catch(const std::exception& e)
    {
        printDashes();
        std::cerr << "\n\nError: " << e.what() << "\n";
        printSyntax();
        return 1;
    }
    
    // after this I must parse the input 

    return 0;
}