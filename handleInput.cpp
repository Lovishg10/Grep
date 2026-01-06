#include <iostream>
#include <string>
#include <vector>
#include <handleInput.h>
// #include <filesystem>

GrepSettings parseInput(int argc, const char* argv[])
{
    GrepSettings settings;

    for (int i = 1; i < argc; ++i)
    {

        std::string_view argument = argv[i];

        if (argument.starts_with('-'))
        {
            size_t len  = argument.length();
            for (size_t j = 1; j < len; ++j)
            {
                char c = argument[j];

                switch (c)
                {
                    case 'i': settings.caseInsesitive = true; break;

                    case 'r': settings.recursive = true;break;
                    
                    default:
                    throw std::runtime_error("Unknown flag: -" + std::string(1, argument[j]));
                }
            }
        }
        else if (settings.pattern.empty()) // if pattern is empty then it should be pattern because file should be last
            settings.pattern = argument;
        else 
            settings.fileNames.push_back(std::string(argument));
    }

    return settings;
}

void checkArguments(int argc = 0)
{
    if (argc < Input::minArgs)
    {
        // Input::lessArgs = true;
        throw std::runtime_error ("Less Arguments than expected");        
    }
}

void printSyntax()
{
    std::cerr << "\nSyntax for Command Line Arguments\n";
    std::cout << Input::syntaxCLI << '\n';
    printDashes();
}

inline void printDashes()
{
    std::cout << "\n" << std::string(Input::dashCount, '-') << "\n\n";
}