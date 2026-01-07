#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>


// User-Made
#include <handleInput.h>
#include <grepEngine.h>

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
        std::cerr << "Error: " << e.what() << '\n';
        
        printSyntax();
        return 1;
    }
    catch(...)
    {
        printDashes();
        std::cerr << "\nUnexpected Error"  << "\n\n";
        
        printDashes();
        return 1;
    }
    
    GrepEngine runtimeEngine;
    
    try 
    {
        runtimeEngine.execute(settings);
    }
    catch(const std::exception& e)
    {
        printDashes();
        std::cerr << "\n\nError: " << e.what() << "\n\n";
        
        printDashes();
        return 1;
    }
    catch(...)
    {
        printDashes();
        std::cerr << "\nUnexpected Error"  << "\n\n";
        
        printDashes();
        return 1;
    }

    return 0;   
}