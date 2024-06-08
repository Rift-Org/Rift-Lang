
#include <iostream>
#include <fstream> // file stream
#include <sstream> // string stream
#include <vector>
#include "../../include/driver/driver.hh"
#include "../../include/error/error.hh"

using namespace rift::error;

namespace rift
{
    namespace driver
    {
        # pragma mark - Driver Tools

        void run(std::string lines, int n)
        {
            std::istringstream scanner(lines);
            std::cout << lines << std::endl;
        }

        void runFile(char* path)
        {
            std::ifstream file(std::string(path), std::ios::binary | std::ios::ate);

            if (file) {
                std::streamsize size = file.tellg();
                file.seekg(0, std::ios::beg);

                std::vector<char> buffer(size);
                if (file.read(buffer.data(), size)) {
                    run(buffer.data(), size);                
                    errorOccured = false; // reset error
                }
            }   
        }

        void runPrompt()
        {
            while(true)
            {
                std::string input = "";
                std::cout << "> " << std::endl;
                std::cin >> input;
                if (input.empty()) break;
                run(input, input.size());
                errorOccured = false; // reset error
            }
        }

        # pragma mark - Driver

        void Driver::run(int argc, char **argv) 
        {
            if (argc > 2) {
                std::cout << "Usage: rift" << std::endl;
            } else if (argc == 2) {
                // file
                runFile(argv[1]);
            } else {
                // prompt
                runPrompt();
            }
        }
    }
}