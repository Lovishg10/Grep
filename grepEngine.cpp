#include <iostream>
#include <string>
#include <string_view>
#include <stdexcept>
#include <fstream>

// USER DEFINED
#include <handleInput.h>
#include <grepEngine.h>
/*  what to do

    I have flag as bool values
    I have fileNames
    I have the pattern

    * so I have to first open the file 
    # --IMPLEMENTATION--

    I make a class with GrepEngine 
*/


/*
    Inside execute , find 
*/

// friend function for class GrepEngine
void changeLength(std::string& temp)
{
    if (temp.length() >= 0 && temp.length() < GrepEngine::lengthPrint)
    {
        std::cout << "INITIATED";

        temp.erase(GrepEngine::lengthPrint, temp.length());

        
    }
}

size_t GrepEngine::lengthPrint = 10;

void GrepEngine::execute(const GrepSettings& settings)
{
    for (const auto& file: settings.fileNames)
    {
        processFile(file, settings);
    }
    std::cout << std::endl;
}

void GrepEngine::processFile(const std::string& file, const GrepSettings& settings)
{
    std::ifstream fileStream(file);

    if (!fileStream.is_open()) 
    {
        throw std::runtime_error ("File - " + file + " not found.");
    }

    std::string line;
    long long int lineNumber = 0;
    size_t printDepth = 20;

    while (std::getline(fileStream, line))
    {
        ++lineNumber;

        size_t foundPos = line.find(settings.pattern);

        if (foundPos != std::string::npos) 
        {
            // Check if line is HUGE (e.g., > 300 chars)
            if (line.length() > GrepEngine::lengthPrint) 
            {
                // Calculate safe start/end points
                // Use 'long long' to allow negative math, then cast back
                size_t start = (foundPos) - printDepth;
                size_t end = (foundPos) + settings.pattern.length() + printDepth;

                // Clamp to boundaries so we don't crash
                if (start < 0) start = 0;
                if (end > line.length()) end = line.length();

                printDashes();
                std::cout << file << ": " << lineNumber << ": ... " 
                        << line.substr(start, end - start) << " ...\n";
                printDashes();
            } 
            else 
            {
                // Normal short line, just print it all
                std::cout << file << ":" << lineNumber << ": " << line << "\n";
            }
        }
    }
}

