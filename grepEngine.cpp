#include <iostream>
#include <string>
#include <string_view>
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <algorithm>

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

namespace fs = std::filesystem; // to use std::filesystem::path
size_t GrepEngine::lengthPrint = 20;


// friend function for class GrepEngine
void changeLength(std::string& temp)
{
    if (temp.length() > GrepEngine::lengthPrint)
    {
        std::cout << "INITIATED";

        temp.erase(GrepEngine::lengthPrint, temp.length());

    }
}


void GrepEngine::execute(const GrepSettings& settings)
{
    try 
    {
        // changing the current dicrectory to its parent to easily access demo.txt files
        // Check if folder exists in current dir first, if not, try parent
        if (fs::exists("Demo_Files")) 
        {
            fs::current_path("Demo_Files");

        } else if (fs::exists(fs::path("..") / "Demo_Files")) 
        {
            fs::current_path(fs::path("..") / "Demo_Files");
        }

        fs::current_path(fs::path("..") / "Demo_Files");
        std::cout << "Changed to parent directory: " << fs::current_path() << std::endl;

    } catch (fs::filesystem_error const& ex) 
    {
        std::cerr << "Error changing directory: " << ex.what() << std::endl;
        std::exit(1);
    }

    for (const auto& file: settings.fileNames)
    {
        processFile(file, settings);
    }


    std::cout << std::endl;
}

bool caseInsensitiveCharCompare(unsigned char ch1, unsigned char ch2) {
    return std::toupper(ch1) == std::toupper(ch2);
    // Note: For full Unicode support, you would need a more robust solution 
    // using std::locale or a library like ICU.
}

std::string::iterator findCaseInsensitive(std::string& haystack, const std::string& needle) {
    auto it = std::search(
        haystack.begin(), haystack.end(),
        needle.begin(), needle.end(),
        caseInsensitiveCharCompare);
    return it;
}

void GrepEngine::processFile(const std::string& file, const GrepSettings& settings)
{
    // std::filesystem::path alias for fs::path
    std::ifstream fileStream(file);

    if (!fileStream.is_open()) 
    {
        throw std::runtime_error ("File - " + file + " not found.");
    }

    std::string line;
    long long int lineNumber = 0;

    while (std::getline(fileStream, line))
    {
        ++lineNumber;

        if (settings.caseInsesitive)
        {
            // doing it 
            auto position = search(line.begin(), line.end(), settings.pattern.begin(),settings.pattern.end(), caseInsensitiveCharCompare);

            if (position != line.end())
            {
                size_t lineLength = line.length();
                // Check if line is HUGE (e.g., > 300 chars)
                if (lineLength > GrepEngine::lengthPrint) 
                {
                    size_t foundPos = std::distance(line.begin(), position);
                //     // Calculate safe start/end points
                //     // Use 'long long' to allow negative math, then cast back 
                    size_t start;
                    size_t end = (foundPos) + settings.pattern.length() + GrepEngine::lengthPrint;
                    
                //     // Clamp to boundaries so we don't crash
                    if (static_cast<long long int>(foundPos) - static_cast<long long int>(GrepEngine::lengthPrint) < 0) 
                    {
                        start = 0;   
                    }
                    else 
                        start = foundPos - GrepEngine::lengthPrint;

                    if (end > lineLength) 
                    {
                        end = lineLength;
                        // std::cout << "END WAS GREATER THAN LENGTH";
                    }
                    
                    

                    // std::cout << end << ' ' << start;
                    printDashes();
                    std::cout << file << ": " << lineNumber << ": ... " 
                            << line.substr(start, end - start) << " ...\n";
                    printDashes();
                }
            }
        }
        else // case-sensitive easy 
        {
            size_t foundPos = line.find(settings.pattern) ;

            if (foundPos != std::string::npos) 
            {
                size_t lineLength = line.length();
                // Check if line is HUGE (e.g., > 300 chars)
                if (lineLength > GrepEngine::lengthPrint) 
                {
                //     // Calculate safe start/end points
                //     // Use 'long long' to allow negative math, then cast back 
                    size_t start;
                    size_t end = (foundPos) + settings.pattern.length() + GrepEngine::lengthPrint;
                    
                //     // Clamp to boundaries so we don't crash
                    if (static_cast<long long int>(foundPos) - static_cast<long long int>(GrepEngine::lengthPrint) < 0) 
                    {
                        start = 0;   
                    }
                    else 
                        start = foundPos - GrepEngine::lengthPrint;

                    if (end > lineLength) 
                    {
                        end = lineLength;
                        // std::cout << "END WAS GREATER THAN LENGTH";
                    }
                    
                

                    // std::cout << end << ' ' << start;
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
}

