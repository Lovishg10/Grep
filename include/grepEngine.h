#ifndef GREP_ENGINE_HEADER
#define GREP_ENGINE_HEADER

#include <string>
#include "handleInput.h"


class GrepEngine
{
    public:
    // Only Public Function to execute everything
    void execute(const GrepSettings& );
    static size_t lengthPrint;

    private:
        void processFile(const std::string& fileName, const GrepSettings& settings);
        void grepWorker(const std::vector<std::string>& allFiles, 
                    size_t start, 
                    size_t end, 
                    const GrepSettings& settings);
        friend void changeLength(std::string& temp);
};

#endif