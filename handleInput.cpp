#include <iostream>
#include <string>
#include <handleInput.h>


void checkArguments(int argc = 0)
{
    if (argc < 3)
    {
        throw std::runtime_error ("Less Arguments than expected");        
    }
}

void printDashes() 
{
    // Create a string with 'n' dashes and print it
    std::cout << '\n' << std::string(dashCount, '-') << '\n';
}

void printSyntax()
{
    std::cerr << "\nSyntax for Command Line Arguments\n";
    std::cout << syntaxCLI << '\n';
    printDashes();
}

