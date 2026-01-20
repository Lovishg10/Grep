#include <iostream>
#include <string>
#include <string_view>
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <thread>
#include <mutex>
// USER DEFINED
#include <handleInput.h>
#include <grepEngine.h>



#include <thread>
#include <algorithm> // For std::max

std::mutex handleThread;
namespace fs = std::filesystem; // to use std::filesystem::path
size_t GrepEngine::lengthPrint = 20;


unsigned int getOptimalThreadCount() {
    
    unsigned int threads = std::thread::hardware_concurrency();

    // 2. Handle the "0" error case
    // If the OS fails to report, default to 2 (safe minimum)
    if (threads == 0) {
        threads = 2;
    }


    threads = std::min(threads, 32u);

    return threads;
}




// friend function for class GrepEngine
void changeLength(std::string& temp)
{
    if (temp.length() > GrepEngine::lengthPrint)
    {
        // std::cout << "INITIATED";
        
        temp.erase(GrepEngine::lengthPrint, temp.length());
        
    }
}

void safePrint(const std::string_view message) {
    std::lock_guard<std::mutex> lock(handleThread);
    std::cout << message;
}
void GrepEngine::grepWorker(const std::vector<std::string>& allFiles, 
                            size_t start, 
                            size_t end, 
                            const GrepSettings& settings) 
{
    
    for (size_t i = start; i < end; ++i) {
        
        // Grab the filename at this index
        const std::string& currentFile = allFiles[i];

        // Do the actual search work
        processFile(currentFile, settings);
    }
}

void GrepEngine::execute(const GrepSettings& settings)
{
    
    if (settings.recursive)
    {
         try 
         {
             
             // Use recursive_directory_iterator for automatic recursion
             for ([[maybe_unused]]const auto& entry : fs::recursive_directory_iterator(fs::current_path())) 
             {   
            // The entry object has useful methods like is_regular_file(), path(), etc.
                if (fs::is_regular_file(entry.status())) 
                {
                    for (const auto& searchFile: settings.fileNames)
                    {

                        auto fileNamePart = entry.path().filename();
                        
                        if (fileNamePart == searchFile)
                        {
                            std::cout << entry.path() << std::endl;

                            
                            processFile(entry.path().string(), settings);
                        }
                        //processFile(file, settings);
                    }
                    
                    
                    // std::cout << "File: " << entry.path().string() << std::endl;
                    
                } 
            }
        } 
        catch (const fs::filesystem_error& e) 
        {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }

    }
    else
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
            safePrint("Changed to parent directory: " + fs::current_path().string() + "\n");
            // std::cout << "Changed to parent directory: " << fs::current_path() << std::endl;
    
        } catch (fs::filesystem_error const& ex) 
        {
            std::cerr << "Error changing directory: " << ex.what() << std::endl;
            std::exit(1);
        }

        const auto& files = settings.fileNames;

        size_t totalFiles = settings.fileNames.size();

        unsigned int threadCount = getOptimalThreadCount();
        if (threadCount > totalFiles) threadCount = static_cast<unsigned int>(totalFiles);
        
        std::vector<std::thread> workers;
        size_t chunkSize = totalFiles / threadCount;

        for (unsigned int i = 0; i < threadCount; ++i) {
            size_t start = i * chunkSize;
            size_t end = start + chunkSize;

            // CRITICAL: The last thread must take "everything remaining"
            // to handle the remainder of the division.
            if (i == threadCount - 1) 
            {
                end = totalFiles;
            }

            // Launch the thread
            // We pass 'this' if processFile is a member function
            workers.emplace_back(&GrepEngine::grepWorker, this, std::cref(files), start, end, std::cref(settings));
        }

        // 5. Wait for everyone to finish
        for (auto& t : workers) 
        {
            if (t.joinable()) t.join();
        }

    }    

    safePrint("\n");
}

bool caseInsensitiveCharCompare(unsigned char ch1, unsigned char ch2) {
    return std::toupper(ch1) == std::toupper(ch2);

}

std::string::iterator findCaseInsensitive(std::string& haystack, const std::string& needle) {
    auto it = std::search(
        haystack.begin(), haystack.end(),
        needle.begin(), needle.end(),
        caseInsensitiveCharCompare);
    return it;
}

// here file can actually be a path of file also (in case of recursive search)
void GrepEngine::processFile(const std::string& file, const GrepSettings& settings)
{
    // std::filesystem::path alias for fs::path
    std::ifstream fileStream(file);
    
    // !settings.recursive make sure that if search is recursive, its fine we donot find the file there
    if (!fileStream.is_open()) 
    {
        if (settings.recursive)
        {
            std::cerr << "File no Found: " << file << '\n';
            return;
        }
        else
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

                    std::string message{file + ": " + std::to_string(lineNumber) + ": ..." + line.substr(start, end - start) + " ...\n"};

                    safePrint(message);
                    // std::cout << file << ": " << lineNumber << ": ... " 
                            // << line.substr(start, end - start) << " ...\n";
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
                    
                    std::string message{file + ": " + std::to_string(lineNumber) + ": ..." + line.substr(start, end - start) + " ...\n"};

                    safePrint(message);
                    // std::cout << file << ": " << lineNumber << ": ... " 
                    //         << line.substr(start, end - start) << " ...\n";
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

