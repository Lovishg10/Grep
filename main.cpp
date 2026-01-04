#include <iostream>
#include <stdexcept>
#include <string_view>
#include <handleInput.h>
#include <vector>

/*
    for grep 
    1. taking argument input
    2. parsing input
    3. file reading and parsing
    4. output based on parsing
*/

int main (int argc, const char*  argv[])
{

    GrepSettings settings;
    try
    { // throws std::runtime exception if less than 3 arguments
        checkArguments(argc);

        settings = parseInput(argc, argv);
    }
    catch(const std::exception& e)
    {
        printDashes();
        std::cerr << "\n\nError: " << e.what() << "\n";
        
        printSyntax();
        return 1;
    }

    return 0;   
}